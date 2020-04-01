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

#include <imagine/gui/AlertView.hh>
#include <imagine/gui/TextEntry.hh>
#include <imagine/base/Base.hh>
#include <emuframework/EmuMainMenuView.hh>
#include <emuframework/EmuSystemActionsView.hh>
#include <emuframework/EmuApp.hh>
#include <emuframework/EmuSystem.hh>
#include <emuframework/RecentGameView.hh>
#include <emuframework/CreditsView.hh>
#include <emuframework/FilePicker.hh>
#include <emuframework/StateSlotView.hh>
#include <emuframework/OptionView.hh>
#include <emuframework/language/language.hh>
#include "EmuOptions.hh"
#include <emuframework/InputManagerView.hh>
#include <emuframework/TouchConfigView.hh>
#include <emuframework/BundledGamesView.hh>
#include "private.hh"
#include "privateInput.hh"
#ifdef CONFIG_BLUETOOTH
#include <imagine/bluetooth/sys.hh>
#include <imagine/bluetooth/BluetoothInputDevScanner.hh>
#endif
#ifdef CONFIG_BLUETOOTH
extern BluetoothAdapter *bta;
#endif

class ResetAlertView : public BaseAlertView
{
public:
	ResetAlertView(ViewAttachParams attach, const char *label):
		BaseAlertView(attach, label,
			[this](const TableView &)
			{
				return 3;
			},
			[this](const TableView &, int idx) -> MenuItem&
			{
				switch(idx)
				{
					default: bug_unreachable("idx == %d", idx); [[fallthrough]];
					case 0: return soft;
					case 1: return hard;
					case 2: return cancel;
				}
			}),
		soft
		{
			get_local_language("Soft Reset"),
			[this](Input::Event e)
			{
				dismiss();
				EmuSystem::reset(EmuSystem::RESET_SOFT);
				emuViewController.showEmulation();
			}
		},
		hard
		{
			get_local_language("Hard Reset"),
			[this](Input::Event e)
			{
				dismiss();
				EmuSystem::reset(EmuSystem::RESET_HARD);
				emuViewController.showEmulation();
			}
		},
		cancel
		{
			get_local_language("Cancel"),
			[this](Input::Event e)
			{
				dismiss();
			}
		}
	{}

protected:
	TextMenuItem soft, hard, cancel;
};

#ifdef CONFIG_BLUETOOTH

static bool initBTAdapter()
{
	if(bta)
	{
		return true;
	}

	logMsg("initializing Bluetooth");
	bta = BluetoothAdapter::defaultAdapter();
	return bta;
}

static auto onScanStatus =
	[](BluetoothAdapter &, uint status, int arg)
	{
		switch(status)
		{
			bcase BluetoothAdapter::INIT_FAILED:
			{
				if(Config::envIsIOS)
				{
					EmuApp::postErrorMessage(get_local_language("BTstack power on failed, make sure the iOS Bluetooth stack is not active"));
				}
			}
			bcase BluetoothAdapter::SCAN_FAILED:
			{
				EmuApp::postErrorMessage(get_local_language("Scan failed"));
			}
			bcase BluetoothAdapter::SCAN_NO_DEVS:
			{
				EmuApp::postMessage(get_local_language("No devices found"));
			}
			bcase BluetoothAdapter::SCAN_PROCESSING:
			{
				EmuApp::printfMessage(2, 0, "Checking %d device(s)...", arg);
			}
			bcase BluetoothAdapter::SCAN_NAME_FAILED:
			{
				EmuApp::postErrorMessage(get_local_language("Failed reading a device name"));
			}
			bcase BluetoothAdapter::SCAN_COMPLETE:
			{
				int devs = Bluetooth::pendingDevs();
				if(devs)
				{
					EmuApp::printfMessage(2, 0, "Connecting to %d device(s)...", devs);
					Bluetooth::connectPendingDevs(bta);
				}
				else
				{
					EmuApp::postMessage(get_local_language("Scan complete, no recognized devices"));
				}
			}
			/*bcase BluetoothAdapter::SOCKET_OPEN_FAILED:
			{
				EmuApp::postErrorMessage(get_local_language("Failed opening a Bluetooth connection"));
			}*/
		}
	};

static void handledFailedBTAdapterInit(ViewAttachParams attach, Input::Event e)
{
	EmuApp::postErrorMessage(get_local_language("Unable to initialize Bluetooth adapter"));
	#ifdef CONFIG_BLUETOOTH_BTSTACK
	if(!FS::exists("/var/lib/dpkg/info/ch.ringwald.btstack.list"))
	{
		auto ynAlertView = std::make_unique<YesNoAlertView>(attach, get_local_language("BTstack not found, open Cydia and install?"));
		ynAlertView->setOnYes(
			[](TextMenuItem &, View &view, Input::Event)
			{
				view.dismiss();
				logMsg("launching Cydia");
				Base::openURL("cydia://package/ch.ringwald.btstack");
			});
		emuViewController.pushAndShowModal(std::move(ynAlertView), e, false);
	}
	#endif
}

#endif

void EmuMainMenuView::onShow()
{
	TableView::onShow();
	logMsg("refreshing main menu state");
	recentGames.setActive(recentGameList.size());
	systemActions.setActive(EmuSystem::gameIsRunning());
	#ifdef CONFIG_BLUETOOTH
	bluetoothDisconnect.setActive(Bluetooth::devsConnected());
	#endif
}

void EmuMainMenuView::loadFileBrowserItems()
{
	item.emplace_back(&loadGame);
	item.emplace_back(&recentGames);
	if(EmuSystem::hasBundledGames && optionShowBundledGames)
	{
		item.emplace_back(&bundledGames);
	}
}

void EmuMainMenuView::loadStandardItems()
{
	item.emplace_back(&systemActions);
	item.emplace_back(&onScreenInputManager);
	item.emplace_back(&inputManager);
	item.emplace_back(&options);
	#ifdef CONFIG_BLUETOOTH
	if(optionShowBluetoothScan)
	{
		item.emplace_back(&scanWiimotes);
		#ifdef CONFIG_BLUETOOTH_SERVER
		item.emplace_back(&acceptPS3ControllerConnection);
		#endif
		item.emplace_back(&bluetoothDisconnect);
	}
	#endif
	item.emplace_back(&benchmark);
	item.emplace_back(&about);
	item.emplace_back(&exitApp);
}

EmuMainMenuView::EmuMainMenuView(ViewAttachParams attach, bool customMenu):
	TableView{appViewTitle(), attach, item},
	loadGame
	{
		get_local_language("Load Game"),
		[this](Input::Event e)
		{
			pushAndShow(EmuFilePicker::makeForLoading(attachParams(), e), e, false);
		}
	},
	systemActions
	{
		get_local_language("System Actions"),
		[this](Input::Event e)
		{
			if(!EmuSystem::gameIsRunning())
				return;
			pushAndShow(makeEmuView(attachParams(), EmuApp::ViewID::SYSTEM_ACTIONS), e);
		}
	},
	recentGames
	{
		get_local_language("Recent Games"),
		[this](Input::Event e)
		{
			if(recentGameList.size())
			{
				pushAndShow(makeView<RecentGameView>(), e);
			}
		}
	},
	bundledGames
	{
		get_local_language("Bundled Games"),
		[this](Input::Event e)
		{
			pushAndShow(makeView<BundledGamesView>(), e);
		}
	},
	options
	{
		get_local_language("Options"),
		[this](Input::Event e)
		{
			pushAndShow(makeView<OptionCategoryView>(), e);
		}
	},
	onScreenInputManager
	{
		get_local_language("On-screen Input Setup"),
		[this](Input::Event e)
		{
			pushAndShow(makeView<TouchConfigView>(vController, EmuSystem::inputFaceBtnName, EmuSystem::inputCenterBtnName), e);
		}
	},
	inputManager
	{
		get_local_language("Key/Gamepad Input Setup"),
		[this](Input::Event e)
		{
			pushAndShow(makeView<InputManagerView>(), e);
		}
	},
	benchmark
	{
		get_local_language("Benchmark Game"),
		[this](Input::Event e)
		{
			pushAndShow(EmuFilePicker::makeForBenchmarking(attachParams(), e), e, false);
		}
	},
	#ifdef CONFIG_BLUETOOTH
	scanWiimotes
	{
		"Scan for Wiimotes/iCP/JS1",
		[this](Input::Event e)
		{
			if(initBTAdapter())
			{
				if(Bluetooth::scanForDevices(*bta, onScanStatus))
				{
					EmuApp::postMessage(4, "Starting Scan...\n(see website for device-specific help)");
				}
				else
				{
					EmuApp::postMessage(1, get_local_language("Still scanning"));
				}
			}
			else
			{
				handledFailedBTAdapterInit(attachParams(), e);
			}
			postDraw();
		}
	},
	bluetoothDisconnect
	{
		get_local_language("Disconnect Bluetooth"),
		[this](Input::Event e)
		{
			if(Bluetooth::devsConnected())
			{
				string_printf(bluetoothDisconnectStr, get_local_language("Really disconnect %d Bluetooth device(s)?"), Bluetooth::devsConnected());
				auto ynAlertView = makeView<YesNoAlertView>(bluetoothDisconnectStr.data());
				ynAlertView->setOnYes(
					[](TextMenuItem &, View &view, Input::Event e)
					{
						view.dismiss();
						Bluetooth::closeBT(bta);
					});
				emuViewController.pushAndShowModal(std::move(ynAlertView), e, false);
			}
		}
	},
	#endif
	#ifdef CONFIG_BLUETOOTH_SERVER
	acceptPS3ControllerConnection
	{
		"Scan for PS3 Controller",
		[this](Input::Event e)
		{
			if(initBTAdapter())
			{
				EmuApp::postMessage(4, get_local_language("Prepare to push the PS button"));
				auto startedScan = Bluetooth::listenForDevices(*bta,
					[this](BluetoothAdapter &bta, uint status, int arg)
					{
						switch(status)
						{
							bcase BluetoothAdapter::INIT_FAILED:
							{
								EmuApp::postErrorMessage(Config::envIsLinux ? 8 : 2,
									Config::envIsLinux ?
										get_local_language("Unable to register server, make sure this executable has cap_net_bind_service enabled and bluetoothd isn't running" :
										"Bluetooth setup failed"));
							}
							bcase BluetoothAdapter::SCAN_COMPLETE:
							{
								EmuApp::postMessage(4, "Push the PS button on your controller\n(see website for pairing help)");
							}
							bdefault: onScanStatus(bta, status, arg);
						}
					});
				if(!startedScan)
				{
					EmuApp::postMessage(1, get_local_language("Still scanning"));
				}
			}
			else
			{
				handledFailedBTAdapterInit(attachParams(), e);
			}
			postDraw();
		}
	},
	#endif
	about
	{
		get_local_language("About"),
		[this](Input::Event e)
		{
			pushAndShow(makeViewWithName<CreditsView>(EmuSystem::creditsViewStr), e);
		}
	},
	exitApp
	{
		get_local_language("Exit"),
		[](Input::Event e)
		{
			Base::exit();
		}
	}
{
	if(!customMenu)
	{
		loadFileBrowserItems();
		loadStandardItems();
		EmuApp::setOnMainMenuItemOptionChanged(
			[this]()
			{
				waitForDrawFinished();
				item.clear();
				loadFileBrowserItems();
				loadStandardItems();
			});
	}
}

OptionCategoryView::OptionCategoryView(ViewAttachParams attach):
	TableView
	{
		get_local_language("Options"),
		attach,
		[this](const TableView &) { return hasGooglePlayStoreFeatures() ? std::size(subConfig) : std::size(subConfig)-1; },
		[this](const TableView &, uint idx) -> MenuItem& { return subConfig[idx]; }
	},
	subConfig
	{
		{
			get_local_language("Video"),
			[this](Input::Event e)
			{
				pushAndShow(makeEmuView(attachParams(), EmuApp::ViewID::VIDEO_OPTIONS), e);
			}
		},
		{
			get_local_language("Audio"),
			[this](Input::Event e)
			{
				pushAndShow(makeEmuView(attachParams(), EmuApp::ViewID::AUDIO_OPTIONS), e);
			}
		},
		{
			get_local_language("System"),
			[this](Input::Event e)
			{
				pushAndShow(makeEmuView(attachParams(), EmuApp::ViewID::SYSTEM_OPTIONS), e);
			}
		},
		{
			get_local_language("GUI"),
			[this](Input::Event e)
			{
				pushAndShow(makeEmuView(attachParams(), EmuApp::ViewID::GUI_OPTIONS), e);
			}
		}
	}
{
	if(hasGooglePlayStoreFeatures())
	{
		subConfig[std::size(subConfig)-1] =
		{
			get_local_language("Beta Testing Opt-in/out"),
			[this]()
			{
				Base::openURL(string_makePrintf<96>("https://play.google.com/apps/testing/%s", appID()).data());
			}
		};
	}
}

std::unique_ptr<View> makeEmuView(ViewAttachParams attach, EmuApp::ViewID id)
{
	auto view = EmuApp::makeCustomView(attach, id);
	if(view)
		return view;
	switch(id)
	{
		case EmuApp::ViewID::MAIN_MENU: return std::make_unique<EmuMainMenuView>(attach);
		case EmuApp::ViewID::SYSTEM_ACTIONS: return std::make_unique<EmuSystemActionsView>(attach);
		case EmuApp::ViewID::VIDEO_OPTIONS: return std::make_unique<VideoOptionView>(attach);
		case EmuApp::ViewID::AUDIO_OPTIONS: return std::make_unique<AudioOptionView>(attach);
		case EmuApp::ViewID::SYSTEM_OPTIONS: return std::make_unique<SystemOptionView>(attach);
		case EmuApp::ViewID::GUI_OPTIONS: return std::make_unique<GUIOptionView>(attach);
		default:
			bug_unreachable("Tried to make non-existing view ID:%d", (int)id);
			return nullptr;
	}
}
