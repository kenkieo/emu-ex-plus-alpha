/*  This file is part of EmuFramework.

	Imagine is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Imagine is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with EmuFramework.  If not, see <http://www.gnu.org/licenses/> */

#include <emuframework/EmuApp.hh>
#include <emuframework/EmuSystem.hh>
#include <emuframework/EmuView.hh>
#include <emuframework/EmuVideoLayer.hh>
#include <emuframework/FilePicker.hh>
#include <imagine/language/language.hh>
#include <imagine/base/Base.hh>
#include <imagine/gui/AlertView.hh>
#include <imagine/gui/ToastView.hh>
#include "EmuOptions.hh"
#include "private.hh"
#include "privateInput.hh"
#include "configFile.hh"
#include "EmuSystemTask.hh"

class AutoStateConfirmAlertView : public YesNoAlertView
{
	std::array<char, 96> msg{};

public:
	AutoStateConfirmAlertView(ViewAttachParams attach, const char *dateStr, bool addToRecent):
		YesNoAlertView
		{
			attach,
			"",
			get_local_language("Continue"),
			get_local_language("Restart Game"),
			[addToRecent](TextMenuItem &, View &view, Input::Event e)
			{
				view.dismiss();
				launchSystem(true, addToRecent);
			},
			[addToRecent](TextMenuItem &, View &view, Input::Event e)
			{
				view.dismiss();
				launchSystem(false, addToRecent);
			}
		}
	{
		string_printf(msg, "Auto-save state exists from:\n%s", dateStr);
		setLabel(msg.data());
	}
};

static std::unique_ptr<AppWindowData> extraWin{};

EmuViewController::EmuViewController(AppWindowData &winData, Gfx::Renderer &renderer, Gfx::RendererTask &rTask,
	VController &vCtrl, EmuVideoLayer &videoLayer, EmuSystemTask &systemTask):
	emuView{{winData.win, rTask}, &videoLayer},
	emuInputView{{winData.win, rTask}, vController},
	popup{{winData.win, rTask}},
	rendererTask_{&rTask},
	systemTask{&systemTask}
{}

static bool shouldExitFromViewRootWithoutPrompt(Input::Event e)
{
	return e.map() == Input::Event::MAP_SYSTEM && (Config::envIsAndroid || Config::envIsLinux);
}

bool EmuMenuViewStack::inputEvent(Input::Event e)
{
	if(e.pushed() && e.isDefaultCancelButton())
	{
		if(size() == 1)
		{
			//logMsg("cancel button at view stack root");
			if(EmuSystem::gameIsRunning() ||
					(!EmuSystem::gameIsRunning() && !shouldExitFromViewRootWithoutPrompt(e)))
			{
				EmuApp::showExitAlert(top().attachParams(), e);
			}
			else
			{
				Base::exit();
			}
			return true;
		}
		else
		{
			popAndShow();
		}
		return true;
	}
	if(e.pushed() && isMenuDismissKey(e))
	{
		if(EmuSystem::gameIsRunning())
		{
			emuViewController.showEmulation();
		}
		return true;
	}
	return ViewStack::inputEvent(e);
}

bool EmuModalViewStack::inputEvent(Input::Event e)
{
	if(ViewStack::inputEvent(e))
		return true;
	if(e.pushed() && e.isDefaultCancelButton())
	{
		popAndShow();
		return true;
	}
	return false;
}

void EmuViewController::initViews(ViewAttachParams viewAttach)
{
	auto &winData = appWindowData(viewAttach.window());
	winData.hasEmuView = true;
	winData.hasPopup = true;
	Base::addOnExit(
		[this](bool backgrounded)
		{
			if(backgrounded)
			{
				if(modalViewController.size())
					modalViewController.top().onHide();
				viewStack.top().onHide();
			}
			return true;
		}, 10);
	Base::addOnResume(
		[this](bool focused)
		{
			if(showingEmulation && focused && EmuSystem::isPaused())
			{
				logMsg("resuming emulation due to app resume");
				#ifdef CONFIG_EMUFRAMEWORK_VCONTROLS
				vController.resetInput();
				#endif
				startEmulation();
			}
			return true;
		}, 10);

	onFrameUpdate = [this](Base::Screen::FrameParams params)
		{
			if(emuVideoInProgress)
			{
				// frame not ready yet, retry on next vblank
				return true;
			}
			bool skipForward = false;
			bool fastForwarding = false;
			if(unlikely(EmuSystem::shouldFastForward()))
			{
				// for skipping loading on disk-based computers
				fastForwarding = true;
				skipForward = true;
				EmuSystem::setSpeedMultiplier(8);
			}
			else if(unlikely(targetFastForwardSpeed > 1))
			{
				fastForwarding = true;
				EmuSystem::setSpeedMultiplier(targetFastForwardSpeed);
			}
			else
			{
				EmuSystem::setSpeedMultiplier(1);
			}
			uint32_t framesAdvanced = EmuSystem::advanceFramesWithTime(params.timestamp());
			if(!framesAdvanced)
				return true;
			if(!optionSkipLateFrames && !fastForwarding)
			{
				framesAdvanced = currentFrameInterval();
			}
			constexpr uint maxFrameSkip = 8;
			uint32_t framesToEmulate = std::min(framesAdvanced, maxFrameSkip);
			emuVideoInProgress = true;
			systemTask->runFrame(&emuVideo, &emuAudio, framesToEmulate, skipForward);
			return true;
		};

	popup.setFace(View::defaultFace);
	{
		auto viewNav = std::make_unique<BasicNavView>
		(
			viewAttach,
			&View::defaultFace,
			&getAsset(emuView.renderer(), ASSET_ARROW),
			&getAsset(emuView.renderer(), ASSET_GAME_ICON)
		);
		viewNav->rotateLeftBtn = true;
		viewNav->setOnPushLeftBtn(
			[this](Input::Event)
			{
				viewStack.popAndShow();
			});
		viewNav->setOnPushRightBtn(
			[this](Input::Event)
			{
				if(EmuSystem::gameIsRunning())
				{
					showEmulation();
				}
			});
		viewNav->showRightBtn(false);
		viewStack.setShowNavViewBackButton(View::needsBackControl);
		EmuApp::onCustomizeNavView(*viewNav);
		viewStack.setNavView(std::move(viewNav));
	}
	{
		auto viewNav = std::make_unique<BasicNavView>
		(
			viewAttach,
			&View::defaultFace,
			&getAsset(emuView.renderer(), ASSET_ARROW),
			nullptr
		);
		viewNav->rotateLeftBtn = true;
		viewNav->setOnPushLeftBtn(
			[this](Input::Event)
			{
				modalViewController.popAndShow();
			});
		modalViewController.setShowNavViewBackButton(View::needsBackControl);
		EmuApp::onCustomizeNavView(*viewNav);
		modalViewController.setNavView(std::move(viewNav));
	}
	modalViewController.setOnRemoveView(
		[this](const ViewStack &controller, View &)
		{
			if(controller.size() == 1 && showingEmulation)
			{
				showEmulation();
			}
		});
	modalViewController.showNavView(optionTitleBar);
	viewStack.showNavView(optionTitleBar);
	emuView.setLayoutInputView(&inputView());
	placeElements();
	pushAndShow(makeEmuView(viewAttach, EmuApp::ViewID::MAIN_MENU), Input::defaultEvent());
	applyFrameRates();
	videoLayer().emuVideo().setOnFrameFinished(
		[this](EmuVideo &)
		{
			emuVideoInProgress = false;
			postDrawToEmuWindows();
		});
	videoLayer().emuVideo().setOnFormatChanged(
		[this, &videoLayer = videoLayer()](EmuVideo &)
		{
			#ifdef CONFIG_GFX_OPENGL_SHADER_PIPELINE
			videoLayer.setEffect(optionImgEffect);
			#else
			videoLayer.resetImage();
			#endif
			if((uint)optionImageZoom > 100)
			{
				placeEmuViews();
			}
		});
}

Base::WindowConfig EmuViewController::addWindowConfig(Base::WindowConfig winConf, AppWindowData &winData)
{
	winConf.setOnInputEvent(
		[this](Base::Window &win, Input::Event e)
		{
			return inputEvent(e);
		});

	winConf.setOnFocusChange(
		[this, &winData](Base::Window &win, uint in)
		{
			winData.focused = in;
			onFocusChange(in);
		});

	winConf.setOnDragDrop(
		[this](Base::Window &win, const char *filename)
		{
			logMsg("got DnD: %s", filename);
			handleOpenFileCommand(filename);
		});

	winConf.setOnSurfaceChange(
		[this, &winData](Base::Window &win, Base::Window::SurfaceChange change)
		{
			rendererTask().updateDrawableForSurfaceChange(winData.drawableHolder, change);
			if(change.resized())
			{
				updateWindowViewport(winData, change);
				emuView.setViewRect(winData.viewport().bounds(), winData.projectionPlane);
				emuInputView.setViewRect(winData.viewport().bounds(), winData.projectionPlane);
				placeElements();
			}
		});

	winConf.setOnDraw(
		[this, &winData](Base::Window &win, Base::Window::DrawParams params)
		{
			if(unlikely(emuVideoInProgress))
			{
				//logMsg("waiting for EmuVideo to signal draw");
				return true;
			}
			popup.prepareDraw();
			emuView.prepareDraw();
			if(!EmuSystem::isActive())
			{
				prepareDraw();
			}
			rendererTask().draw(winData.drawableHolder, win, params, {},
				[this, &winData](Gfx::Drawable &drawable, Base::Window &win, Gfx::SyncFence fence, Gfx::RendererDrawTask task)
				{
					auto cmds = task.makeRendererCommands(drawable, winData.viewport(), winData.projectionMat);
					cmds.clear();
					cmds.waitSync(fence);
					drawMainWindow(win, cmds, winData.hasEmuView, winData.hasPopup);
				});
			return false;
		});

	return winConf;
}

void EmuViewController::pushAndShow(std::unique_ptr<View> v, Input::Event e, bool needsNavView)
{
	showUI(false);
	viewStack.pushAndShow(std::move(v), e, needsNavView);
}

void EmuViewController::pop()
{
	popModalViews();
	viewStack.pop();
}

void EmuViewController::dismissView(View &v)
{
	viewStack.dismissView(v);
}

bool EmuViewController::inputEvent(Input::Event e)
{
	if(e.isPointer())
	{
		//logMsg("Pointer %s @ %d,%d", e.actionToStr(e.state), e.x, e.y);
	}
	else
	{
		//logMsg("%s %s from %s", e.device->keyName(e.button), e.actionToStr(e.state), e.device->name());
	}
	if(likely(EmuSystem::isActive()))
	{
		return emuInputView.inputEvent(e);
	}
	else if(modalViewController.size())
		return modalViewController.inputEvent(e);
	else if(!showingEmulation)
		return viewStack.inputEvent(e);
	return false;
}

void EmuViewController::movePopupToWindow(Base::Window &win)
{
	auto &origWin = popup.window();
	if(origWin == win)
		return;
	auto &origWinData = appWindowData(origWin);
	origWinData.hasPopup = false;
	auto &winData = appWindowData(win);
	winData.hasPopup = true;
	popup.setWindow(&win);
}

void EmuViewController::moveEmuViewToWindow(Base::Window &win)
{
	auto &origWin = emuView.window();
	if(origWin == win)
		return;
	auto &origWinData = appWindowData(origWin);
	origWinData.hasEmuView = false;
	auto &winData = appWindowData(win);
	winData.hasEmuView = true;
	emuView.setWindow(&win);
}

void EmuViewController::configureAppForEmulation(bool running)
{
	Base::setIdleDisplayPowerSave(running ? (bool)optionIdleDisplayPowerSave : true);
	applyOSNavStyle(running);
	Input::setKeyRepeat(!running);
}

void EmuViewController::configureWindowForEmulation(Base::Window &win, bool running)
{
	#if defined CONFIG_BASE_SCREEN_FRAME_INTERVAL
	win.screen()->setFrameInterval(optionFrameInterval);
	#endif
	emuView.renderer().setWindowValidOrientations(win, running ? optionGameOrientation : optionMenuOrientation);
	win.screen()->setFrameRate(running ? EmuSystem::frameRate() : Base::Screen::DISPLAY_RATE_DEFAULT);
	movePopupToWindow(running ? emuView.window() : emuInputView.window());
}

void EmuViewController::showEmulation()
{
	if(showingEmulation)
		return;
	viewStack.top().onHide();
	showingEmulation = true;
	configureAppForEmulation(true);
	configureWindowForEmulation(emuView.window(), true);
	commonInitInput();
	popup.clear();
	emuInputView.resetInput();
	emuInputView.postDraw();
	startEmulation();
	placeEmuViews();
}

void EmuViewController::showUI(bool updateTopView)
{
	if(!showingEmulation)
		return;
	showingEmulation = false;
	pauseEmulation();
	configureAppForEmulation(false);
	configureWindowForEmulation(emuView.window(), false);
	emuView.postDraw();
	if(updateTopView)
	{
		viewStack.show();
		viewStack.top().postDraw();
	}
}

bool EmuViewController::showAutoStateConfirm(Input::Event e, bool addToRecent)
{
	if(!(optionConfirmAutoLoadState && optionAutoSaveState))
	{
		return false;
	}
	auto saveStr = EmuSystem::sprintStateFilename(-1);
	if(FS::exists(saveStr))
	{
		auto mTime = FS::status(saveStr).lastWriteTimeLocal();
		char dateStr[64]{};
		std::strftime(dateStr, sizeof(dateStr), strftimeFormat, &mTime);
		pushAndShowModal(std::make_unique<AutoStateConfirmAlertView>(viewStack.top().attachParams(), dateStr, addToRecent), e, false);
		return true;
	}
	return false;
}

void EmuViewController::placeEmuViews()
{
	emuView.place();
	emuInputView.place();
}

void EmuViewController::placeElements()
{
	//logMsg("placing app elements");
	{
		auto &winData = appWindowData(popup.window());
		popup.setViewRect(winData.viewport().bounds(), winData.projectionPlane);
		popup.place();
	}
	auto &winData = mainWindowData();
	TableView::setDefaultXIndent(inputView().window(), winData.projectionPlane);
	placeEmuViews();
	viewStack.place(winData.viewport().bounds(), winData.projectionPlane);
	modalViewController.place(winData.viewport().bounds(), winData.projectionPlane);
}

void EmuViewController::setEmuViewOnExtraWindow(bool on, Base::Screen &screen)
{
	if(on && !extraWin)
	{
		logMsg("setting emu view on extra window");
		extraWin = std::make_unique<AppWindowData>();
		Base::WindowConfig winConf;
		winConf.setScreen(screen);

		winConf.setOnSurfaceChange(
			[this, &winData = *extraWin](Base::Window &win, Base::Window::SurfaceChange change)
			{
				rendererTask().updateDrawableForSurfaceChange(winData.drawableHolder, change);
				if(change.resized())
				{
					logMsg("view resize for extra window");
					updateProjection(winData, makeViewport(win));
					emuView.setViewRect(winData.viewport().bounds(), winData.projectionPlane);
					emuView.place();
				}
			});

		winConf.setOnDraw(
			[this, &winData = *extraWin](Base::Window &win, Base::Window::DrawParams params)
			{
				if(unlikely(emuVideoInProgress))
				{
					//logMsg("waiting for EmuVideo to signal draw");
					return true;
				}
				if(winData.hasPopup)
				{
					popup.prepareDraw();
				}
				emuView.prepareDraw();
				rendererTask().draw(winData.drawableHolder, win, params, {},
					[this, &winData](Gfx::Drawable &drawable, Base::Window &win, Gfx::SyncFence fence, Gfx::RendererDrawTask task)
					{
						auto cmds = task.makeRendererCommands(drawable, winData.viewport(), winData.projectionMat);
						cmds.clear();
						cmds.waitSync(fence);
						emuView.draw(cmds);
						if(winData.hasPopup)
						{
							popup.draw(cmds);
						}
						cmds.present();
					});
				return false;
			});

		winConf.setOnInputEvent(
			[this](Base::Window &win, Input::Event e)
			{
				if(likely(EmuSystem::isActive()) && e.isKey())
				{
					return emuInputView.inputEvent(e);
				}
				return false;
			});

		winConf.setOnFocusChange(
			[this, &winData = *extraWin](Base::Window &win, uint in)
			{
				winData.focused = in;
				onFocusChange(in);
			});

		winConf.setOnDismissRequest(
			[](Base::Window &win)
			{
				win.dismiss();
			});

		winConf.setOnDismiss(
			[this](Base::Window &win)
			{
				EmuSystem::resetFrameTime();
				logMsg("setting emu view on main window");
				auto &mainWin = mainWindowData();
				moveEmuViewToWindow(mainWin.win);
				movePopupToWindow(mainWin.win);
				emuView.setLayoutInputView(&inputView());
				placeEmuViews();
				mainWin.win.postDraw();
				if(EmuSystem::isActive() && mainWin.win.screen() != win.screen())
				{
					win.screen()->removeOnFrame(onFrameUpdate);
					mainWin.win.screen()->addOnFrame(onFrameUpdate);
					applyFrameRates();
				}
			});

		winConf.setOnFree(
			[]()
			{
				extraWin.reset();
			});

		winConf.setCustomData(extraWin.get());
		emuView.renderer().initWindow(extraWin->win, winConf);
		extraWin->focused = true;
		logMsg("init extra window");
		auto &mainWin = mainWindowData();
		if(EmuSystem::isActive() && mainWin.win.screen() != extraWin->win.screen())
		{
			mainWin.win.screen()->removeOnFrame(onFrameUpdate);
			extraWin->win.screen()->addOnFrame(onFrameUpdate);
			applyFrameRates();
		}
		moveEmuViewToWindow(extraWin->win);
		emuView.setLayoutInputView(nullptr);
		updateProjection(*extraWin, makeViewport(extraWin->win));
		extraWin->win.setTitle(appName());
		extraWin->win.show();
		placeEmuViews();
		mainWin.win.postDraw();
	}
	else if(!on && extraWin)
	{
		extraWin->win.dismiss();
	}
}

void EmuViewController::startViewportAnimation(AppWindowData &winData)
{
	auto oldViewport = winData.viewport();
	auto newViewport = makeViewport(winData.win);
	winData.animatedViewport.start(winData.win, oldViewport, newViewport);
	winData.win.postDraw();
}

void EmuViewController::startMainViewportAnimation()
{
	startViewportAnimation(mainWindowData());
}

void EmuViewController::updateWindowViewport(AppWindowData &winData, Base::Window::SurfaceChange change)
{
	if(change.surfaceResized())
	{
		winData.animatedViewport.cancel();
		updateProjection(winData, makeViewport(winData.win));
	}
	else if(change.contentRectResized())
	{
		startViewportAnimation(winData);
	}
	else if(change.customViewportResized())
	{
		updateProjection(winData, winData.animatedViewport.viewport());
	}
}

void EmuViewController::updateEmuAudioStats(uint underruns, uint overruns, uint callbacks, double avgCallbackFrames, uint frames)
{
	emuView.updateAudioStats(underruns, overruns, callbacks, avgCallbackFrames, frames);
}

void EmuViewController::clearEmuAudioStats()
{
	emuView.clearAudioStats();
}

bool EmuViewController::allWindowsAreFocused() const
{
	return mainWindowData().focused && (!extraWin || extraWin->focused);
}

void EmuViewController::applyFrameRates()
{
	EmuSystem::setFrameTime(EmuSystem::VIDSYS_NATIVE_NTSC,
		optionFrameRate.val ? IG::FloatSeconds(optionFrameRate.val) : emuView.window().screen()->frameTime());
	EmuSystem::setFrameTime(EmuSystem::VIDSYS_PAL,
		optionFrameRatePAL.val ? IG::FloatSeconds(optionFrameRatePAL.val) : emuView.window().screen()->frameTime());
	EmuSystem::configFrameTime(optionSoundRate);
}

static uint initialDelayFrames(Base::Screen &screen)
{
	return std::round(screen.frameRate() / 5.);
}

void EmuViewController::addInitialOnFrame(Base::Screen &screen, uint delay)
{
	screen.addOnFrame(
		[this, delay](Base::Screen::FrameParams params)
		{
			if(!EmuSystem::isActive())
				return false;
			postDrawToEmuWindows();
			// delay for timestamps to stabilize
			if(delay)
			{
				addInitialOnFrame(params.screen(), delay - 1);
			}
			else
			{
				params.screen().addOnFrame(onFrameUpdate);
			}
			return false;
		});
}

void EmuViewController::startEmulation()
{
	setCPUNeedsLowLatency(true);
	systemTask->start();
	EmuSystem::start();
	videoLayer().setBrightness(1.f);
	auto &screen = *emuView.window().screen();
	addInitialOnFrame(screen, initialDelayFrames(screen));
}

void EmuViewController::pauseEmulation()
{
	setCPUNeedsLowLatency(false);
	systemTask->pause();
	EmuSystem::pause();
	videoLayer().setBrightness(showingEmulation ? .75f : .25f);
	setFastForwardActive(false);
	emuVideoInProgress = false;
	emuView.window().screen()->removeOnFrame(onFrameUpdate);
}

void EmuViewController::closeSystem(bool allowAutosaveState)
{
	showUI();
	systemTask->stop();
	emuVideo.clear();
	EmuSystem::closeRuntimeSystem(allowAutosaveState);
	viewStack.navView()->showRightBtn(false);
	if(int idx = viewStack.viewIdx(get_local_language("System Actions"));
		idx > 0)
	{
		viewStack.popTo(viewStack.viewAtIdx(idx - 1));
	}
}

void EmuViewController::postDrawToEmuWindows()
{
	emuView.window().postDraw();
}

Base::Screen *EmuViewController::emuWindowScreen() const
{
	return emuView.window().screen();
}

Base::Window &EmuViewController::emuWindow() const
{
	return emuView.window();
}

Gfx::RendererTask &EmuViewController::rendererTask() const
{
	return *rendererTask_;
}

void EmuViewController::pushAndShowModal(std::unique_ptr<View> v, Input::Event e, bool needsNavView)
{
	showUI();
	viewStack.top().onHide();
	modalViewController.pushAndShow(std::move(v), e, needsNavView);
}

bool EmuViewController::hasModalView()
{
	return modalViewController.size();
}

void EmuViewController::popModalViews()
{
	modalViewController.popAll();
}

void EmuViewController::prepareDraw()
{
	modalViewController.prepareDraw();
	viewStack.prepareDraw();
}

void EmuViewController::drawMainWindow(Base::Window &win, Gfx::RendererCommands &cmds, bool hasEmuView, bool hasPopup)
{
	if(showingEmulation)
	{
		if(hasEmuView)
		{
			emuView.draw(cmds);
		}
		emuInputView.draw(cmds);
		if(hasPopup)
			popup.draw(cmds);
	}
	else
	{
		if(hasEmuView)
		{
			emuView.draw(cmds);
		}
		if(modalViewController.size())
			modalViewController.draw(cmds);
		else
			viewStack.draw(cmds);
		popup.draw(cmds);
	}
	cmds.present();
}

void EmuViewController::popTo(View &v)
{
	popModalViews();
	viewStack.popTo(v);
}

void EmuViewController::popToRoot()
{
	popModalViews();
	viewStack.popToRoot();
}

void EmuViewController::showNavView(bool show)
{
	viewStack.showNavView(show);
}

void EmuViewController::setShowNavViewBackButton(bool show)
{
	viewStack.setShowNavViewBackButton(show);
}

void EmuViewController::showSystemActionsView(ViewAttachParams attach, Input::Event e)
{
	showUI();
	if(!viewStack.contains(get_local_language("System Actions")))
	{
		viewStack.pushAndShow(makeEmuView(attach, EmuApp::ViewID::SYSTEM_ACTIONS), e);
	}
}

void EmuViewController::onInputDevicesChanged()
{
	#ifdef CONFIG_BLUETOOTH
	if(viewStack.size() == 1) // update bluetooth items
		viewStack.top().onShow();
	#endif
}

void EmuViewController::onSystemCreated()
{
	viewStack.navView()->showRightBtn(true);
}

EmuInputView &EmuViewController::inputView()
{
	return emuInputView;
}

ToastView &EmuViewController::popupMessageView()
{
	return popup;
}

EmuVideoLayer &EmuViewController::videoLayer() const
{
	return *emuView.videoLayer();
}

void EmuViewController::onScreenChange(Base::Screen &screen, Base::Screen::Change change)
{
	if(change.added())
	{
		logMsg("screen added");
		if(optionShowOnSecondScreen && screen.screens() > 1)
			setEmuViewOnExtraWindow(true, screen);
	}
	else if(change.removed())
	{
		logMsg("screen removed");
		if(extraWin && *extraWin->win.screen() == screen)
			setEmuViewOnExtraWindow(false, screen);
	}
}

void EmuViewController::handleOpenFileCommand(const char *path)
{
	auto type = FS::status(path).type();
	if(type == FS::file_type::directory)
	{
		logMsg("changing to dir %s from external command", path);
		showUI(false);
		popToRoot();
		string_copy(lastLoadPath, path);
		pushAndShow(EmuFilePicker::makeForLoading(viewStack.top().attachParams(), Input::defaultEvent()), Input::defaultEvent(), false);
		return;
	}
	if(type != FS::file_type::regular || (!EmuApp::hasArchiveExtension(path) && !EmuSystem::defaultFsFilter(path)))
	{
		logMsg("unrecognized file type");
		return;
	}
	logMsg("opening file %s from external command", path);
	showUI();
	popToRoot();
	onSelectFileFromPicker(path, Input::Event{});
}

void EmuViewController::onFocusChange(uint in)
{
	if(showingEmulation)
	{
		if(in && EmuSystem::isPaused())
		{
			logMsg("resuming emulation due to window focus");
			#ifdef CONFIG_EMUFRAMEWORK_VCONTROLS
			vController.resetInput();
			#endif
			startEmulation();
			postDrawToEmuWindows();
		}
		else if(optionPauseUnfocused && !EmuSystem::isPaused() && !allWindowsAreFocused())
		{
			logMsg("pausing emulation with all windows unfocused");
			pauseEmulation();
			postDrawToEmuWindows();
		}
	}
}

void EmuViewController::setOnScreenControls(bool on)
{
	emuInputView.setTouchControlsOn(on);
	placeEmuViews();
}

void EmuViewController::updateAutoOnScreenControlVisible()
{
	#ifdef CONFIG_VCONTROLS_GAMEPAD
	if((uint)optionTouchCtrl == 2)
	{
		if(emuInputView.touchControlsAreOn() && physicalControlsPresent)
		{
			logMsg("auto-turning off on-screen controls");
			setOnScreenControls(0);
		}
		else if(!emuInputView.touchControlsAreOn() && !physicalControlsPresent)
		{
			logMsg("auto-turning on on-screen controls");
			setOnScreenControls(1);
		}
	}
	#endif
}

void EmuViewController::setPhysicalControlsPresent(bool present)
{
	physicalControlsPresent = present;
	if(present)
	{
		logMsg("Physical controls are present");
	}
}

AppWindowData &EmuViewController::mainWindowData() const
{
	return appWindowData(emuInputView.window());
}

void EmuViewController::setFastForwardActive(bool active)
{
	targetFastForwardSpeed = active ? optionFastForwardSpeed.val : 0;
	emuAudio.setAddSoundBuffersOnUnderrun(active ? optionAddSoundBuffersOnUnderrun.val : false);
}
