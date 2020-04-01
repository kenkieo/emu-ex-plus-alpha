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

#include <emuframework/TouchConfigView.hh>
#include "EmuOptions.hh"
#include <imagine/gui/AlertView.hh>
#include <imagine/base/Timer.hh>
#include <imagine/input/DragTracker.hh>
#include <utility>
#include "private.hh"
#include "privateInput.hh"

static constexpr bool CAN_TURN_OFF_MENU_BTN = !Config::envIsIOS;

static const char *ctrlStateStr[]
{
	get_local_language("Off"),
	get_local_language("On"),
	get_local_language("Hidden")
};

static const char *touchCtrlSizeMenuName[10]
{
	"6.5",
	"7",
	"7.5",
	"8",
	"8.5",
	"9",
	"10",
	"12",
	"14",
	"15",
};

static const uint touchCtrlSizeMenuVal[10]
{
	650,
	700,
	750,
	800,
	850,
	900,
	1000,
	1200,
	1400,
	1500,
};

static const char *touchDpadDeadzoneMenuName[3]
{
	"1", "1.35", "1.6"
};

static const uint touchDpadDeadzoneMenuVal[3]
{
	100, 135, 160
};

static const char *touchDpadDiagonalSensitivityMenuName[5]
{
	get_local_language("None"),
	get_local_language("Low"),
	 get_local_language("M-Low"),
	 get_local_language("Med."),
	 get_local_language("High")
};

static const uint touchDpadDiagonalSensitivityMenuVal[5]
{
	1000, 1500, 1750, 2000, 2500
};

static const char *touchCtrlBtnSpaceMenuName[4]
{
	"1", "2", "3", "4"
};

static const uint touchCtrlBtnSpaceMenuVal[4]
{
	100, 200, 300, 400
};

static const char *touchCtrlExtraXBtnSizeMenuName[4]
{
	get_local_language("None"), get_local_language("Gap only"), "10%", "25%"
};

static const uint touchCtrlExtraXBtnSizeMenuVal[4]
{
	0, 1, 200, 500
};

static const char *touchCtrlExtraYBtnSizeMenuName[4]
{
	get_local_language("None"), get_local_language("Gap only"), "25%", "50%"
};

static const uint touchCtrlExtraYBtnSizeMenuVal[4]
{
	0, 1, 500, 1000
};

static const char *alphaMenuName[6]
{
	"0%", "10%", "25%", "50%", "65%", "75%"
};

static const uint alphaMenuVal[6]
{
	0, int(255 * .1), int(255 * .25), int(255 * .5), int(255 * .65), int(255 * .75)
};

static auto &layoutPosArr(Base::Window &win)
{
	return vController.layoutPosition()[win.isPortrait() ? 1 : 0];
}

class OnScreenInputPlaceView : public View
{
	struct DragState
	{
		constexpr DragState() {};
		int elem = -1;
		IG::Point2D<int> startPos;
	};
	Gfx::Text text;
	TimedInterpolator<float> textFade;
	Base::Timer animationStartTimer{"OnScreenInputPlaceView::animationStartTimer"};
	Base::Screen::OnFrameDelegate animate;
	IG::WindowRect exitBtnRect;
	DragState drag[Config::Input::MAX_POINTERS];
	Input::DragTracker dragTracker{};

public:
	OnScreenInputPlaceView(ViewAttachParams attach): View(attach) {}
	~OnScreenInputPlaceView();
	void init();
	void place() final;
	bool inputEvent(Input::Event e) final;
	void draw(Gfx::RendererCommands &cmds) final;
	void onAddedToController(Input::Event e) final {}
};

void OnScreenInputPlaceView::init()
{
	applyOSNavStyle(true);
	text = {get_local_language("Click center to go back"), &View::defaultFace};
	textFade.set(1.);
	animate =
		[this](Base::Screen::FrameParams params)
		{
			postDraw();
			//logMsg("updating fade");
			return textFade.update(1);
		};
	animationStartTimer.callbackAfterSec(
		[this]()
		{
			logMsg("starting fade");
			textFade.set(1., 0., INTERPOLATOR_TYPE_LINEAR, 25);
			screen()->addOnFrame(animate);
		}, 2, {});
}

OnScreenInputPlaceView::~OnScreenInputPlaceView()
{
	applyOSNavStyle(false);
	animationStartTimer.deinit();
	screen()->removeOnFrame(animate);
}

void OnScreenInputPlaceView::place()
{
	for(auto &d : drag)
	{
		d.elem = -1;
	}

	auto exitBtnPos = viewRect().pos(C2DO);
	int exitBtnSize = window().widthSMMInPixels(10.);
	exitBtnRect = IG::makeWindowRectRel(exitBtnPos - IG::WP{exitBtnSize/2, exitBtnSize/2}, {exitBtnSize, exitBtnSize});
	text.compile(renderer(), projP);
}

bool OnScreenInputPlaceView::inputEvent(Input::Event e)
{
	if(!e.isPointer())
	{
		if(e.pushed())
		{
			dismiss();
			return true;
		}
		return false;
	}
	if(e.pushed() && !textFade.duration())
	{
		animationStartTimer.deinit();
		logMsg("starting fade");
		textFade.set(1., 0., INTERPOLATOR_TYPE_LINEAR, 20);
		screen()->addOnFrame(animate);
	}
	auto &d = drag[e.deviceID()];
	dragTracker.inputEvent(e,
		[&](Input::DragTrackerState)
		{
			if(d.elem == -1)
			{
				iterateTimes(vController.numElements(), i)
				{
					if(vController.state(i) != 0 && vController.bounds(i).contains(e.pos()))
					{
						for(auto &otherDrag : drag)
						{
							if(otherDrag.elem == (int)i)
								continue; // element already grabbed
						}
						d.elem = i;
						d.startPos = vController.bounds(d.elem).pos(C2DO);
						break;
					}
				}
			}
		},
		[&](Input::DragTrackerState state, Input::DragTrackerState)
		{
			if(d.elem >= 0)
			{
				auto newPos = d.startPos + state.downPosDiff();
				waitForDrawFinished();
				vController.setPos(d.elem, newPos);
				auto layoutPos = vControllerPixelToLayoutPos(vController.bounds(d.elem).pos(C2DO), vController.bounds(d.elem).size(), viewRect());
				//logMsg("set pos %d,%d from %d,%d", layoutPos.pos.x, layoutPos.pos.y, layoutPos.origin.xScaler(), layoutPos.origin.yScaler());
				auto &vCtrlLayoutPos = vController.layoutPosition()[window().isPortrait() ? 1 : 0];
				vCtrlLayoutPos[d.elem].origin = layoutPos.origin;
				vCtrlLayoutPos[d.elem].pos = layoutPos.pos;
				vController.setLayoutPositionChanged();
				emuViewController.placeEmuViews();
				postDraw();
			}
		},
		[&](Input::DragTrackerState state)
		{
			if(d.elem >= 0)
			{
				d.elem = -1;
			}
			else if(exitBtnRect.overlaps(state.pos()) && exitBtnRect.overlaps(state.downPos()))
			{
				dismiss();
			}
		});
	return true;
}

void OnScreenInputPlaceView::draw(Gfx::RendererCommands &cmds)
{
	using namespace Gfx;
	projP.resetTransforms(cmds);
	vController.draw(cmds, true, false, true, .75);
	cmds.setColor(.5, .5, .5);
	cmds.setCommonProgram(CommonProgram::NO_TEX, projP.makeTranslate());
	Gfx::GC lineSize = projP.unprojectYSize(1);
	GeomRect::draw(cmds, Gfx::GCRect{-projP.wHalf(), -lineSize/(Gfx::GC)2.,
		projP.wHalf(), lineSize/(Gfx::GC)2.});
	lineSize = projP.unprojectYSize(1);
	GeomRect::draw(cmds, Gfx::GCRect{-lineSize/(Gfx::GC)2., -projP.hHalf(),
		lineSize/(Gfx::GC)2., projP.hHalf()});

	if(textFade.now() != 0.)
	{
		cmds.setColor(0., 0., 0., textFade.now()/2.);
		GeomRect::draw(cmds, Gfx::makeGCRectRel({-text.xSize/(Gfx::GC)2. - text.spaceSize, -text.ySize/(Gfx::GC)2. - text.spaceSize},
			{text.xSize + text.spaceSize*(Gfx::GC)2., text.ySize + text.spaceSize*(Gfx::GC)2.}));
		cmds.setColor(1., 1., 1., textFade.now());
		cmds.setCommonProgram(CommonProgram::TEX_ALPHA);
		text.draw(cmds, projP.unProjectRect(viewRect()).pos(C2DO), C2DO, projP);
	}
}

template <class T, class T2, size_t S>
static int findIdxInArray(T (&arr)[S], const T2 &val)
{
	for(const auto &e : arr)
	{
		if(val == e)
		{
			return &e - arr;
		}
	}
	return -1;
}

template <class T, class T2, size_t S>
static int findIdxInArrayOrDefault(T (&arr)[S], const T2 &val, int defaultIdx)
{
	int idx = findIdxInArray(arr, val);
	if(idx == -1)
		return defaultIdx;
	return idx;
}

#ifdef CONFIG_VCONTROLS_GAMEPAD
static void setPointerInputPlayer(uint val)
{
	pointerInputPlayer = val;
	vController.updateMapping(pointerInputPlayer);
}

void TouchConfigView::setSize(uint16_t val)
{
	string_printf(sizeStr, "%.2f", val / 100.);
	optionTouchCtrlSize = val;
	EmuControls::setupVControllerVars();
	vController.place();
}

static void setDeadzone(uint val)
{
	optionTouchDpadDeadzone = val;
	vController.gamePad().dPad().setDeadzone(emuVideo.renderer(), vController.xMMSizeToPixel(Base::mainWindow(), int(optionTouchDpadDeadzone) / 100.), vController.windowData());
}

static void setDiagonalSensitivity(uint val)
{
	optionTouchDpadDiagonalSensitivity = val;
	vController.gamePad().dPad().setDiagonalSensitivity(emuVideo.renderer(), optionTouchDpadDiagonalSensitivity / 1000., vController.windowData());
}

static void setButtonSpace(uint val)
{
	optionTouchCtrlBtnSpace = val;
	EmuControls::setupVControllerVars();
	vController.place();
}

static void setButtonExtraXSize(uint val)
{
	optionTouchCtrlExtraXBtnSize = val;
	EmuControls::setupVControllerVars();
	vController.place();
}

static void setButtonExtraYSizeMultiRow(uint val)
{
	optionTouchCtrlExtraYBtnSizeMultiRow = val;
	EmuControls::setupVControllerVars();
	vController.place();
}

static void setButtonExtraYSize(uint val)
{
	optionTouchCtrlExtraYBtnSize = val;
	EmuControls::setupVControllerVars();
	vController.place();
}

static void setButtonStagger(uint val)
{
	optionTouchCtrlBtnStagger = val;
	EmuControls::setupVControllerVars();
	vController.place();
}
#endif

static void setButtonState(uint state, uint btnIdx, Base::Window &win)
{
	vController.layoutPosition()[win.isPortrait() ? 1 : 0][btnIdx].state = state;
	vController.setLayoutPositionChanged();
	EmuControls::setupVControllerVars();
}

static void setAlpha(uint val)
{
	optionTouchCtrlAlpha = val;
	vController.setAlpha((int)optionTouchCtrlAlpha / 255.0);
}

void TouchConfigView::draw(Gfx::RendererCommands &cmds)
{
	using namespace Gfx;
	projP.resetTransforms(cmds);
	vController.draw(cmds, true, false, true, .75);
	TableView::draw(cmds);
}

void TouchConfigView::place()
{
	refreshTouchConfigMenu();
	TableView::place();
}

void TouchConfigView::refreshTouchConfigMenu()
{
	auto &layoutPos = layoutPosArr(window());
	alpha.setSelected(findIdxInArrayOrDefault(alphaMenuVal, optionTouchCtrlAlpha.val, 3), *this);
	ffState.setSelected(layoutPos[4].state, *this);
	menuState.setSelected(layoutPos[3].state - (CAN_TURN_OFF_MENU_BTN ? 0 : 1), *this);
	#ifdef CONFIG_VCONTROLS_GAMEPAD
	touchCtrl.setSelected((int)optionTouchCtrl, *this);
	if(EmuSystem::maxPlayers > 1)
		pointerInput.setSelected((int)pointerInputPlayer, *this);
	size.setSelected(findIdxInArrayOrDefault(touchCtrlSizeMenuVal, (uint)optionTouchCtrlSize, std::size(sizeItem) - 1), *this);
	dPadState.setSelected(layoutPos[0].state, *this);
	faceBtnState.setSelected(layoutPos[2].state, *this);
	centerBtnState.setSelected(layoutPos[1].state, *this);
	if(vController.hasTriggers())
	{
		lTriggerState.setSelected(layoutPos[5].state, *this);
		rTriggerState.setSelected(layoutPos[6].state, *this);
	}
	deadzone.setSelected(findIdxInArray(touchDpadDeadzoneMenuVal, (uint)optionTouchDpadDeadzone), *this);
	diagonalSensitivity.setSelected(findIdxInArray(touchDpadDiagonalSensitivityMenuVal, (uint)optionTouchDpadDiagonalSensitivity), *this);
	btnSpace.setSelected(findIdxInArray(touchCtrlBtnSpaceMenuVal, (uint)optionTouchCtrlBtnSpace), *this);
	btnExtraXSize.setSelected(findIdxInArray(touchCtrlExtraXBtnSizeMenuVal, (uint)optionTouchCtrlExtraXBtnSize), *this);
	if(EmuSystem::inputFaceBtns < 4 || (EmuSystem::inputFaceBtns == 6 && !EmuSystem::inputHasTriggerBtns))
	{
		btnExtraYSize.setSelected(findIdxInArray(touchCtrlExtraYBtnSizeMenuVal, (uint)optionTouchCtrlExtraYBtnSize), *this);
	}
	if(EmuSystem::inputFaceBtns >= 4)
	{
		btnExtraYSizeMultiRow.setSelected(findIdxInArray(touchCtrlExtraXBtnSizeMenuVal, (uint)optionTouchCtrlExtraYBtnSizeMultiRow), *this);
	}
	btnStagger.setSelected(optionTouchCtrlBtnStagger, *this);
	boundingBoxes.setBoolValue((int)optionTouchCtrlBoundingBoxes, *this);
	if(!optionVibrateOnPush.isConst)
	{
		vibrate.setBoolValue((int)optionVibrateOnPush, *this);
	}
	showOnTouch.setBoolValue((int)optionTouchCtrlShowOnTouch, *this);
		#ifdef CONFIG_BASE_ANDROID
		useScaledCoordinates.setBoolValue(optionTouchCtrlScaledCoordinates, *this);
		#endif
	#endif
}

TouchConfigView::TouchConfigView(ViewAttachParams attach, VController &vController, const char *faceBtnName, const char *centerBtnName):
	TableView{get_local_language("On-screen Input Setup"), attach, item},
	vController{vController},
	#ifdef CONFIG_VCONTROLS_GAMEPAD
	touchCtrlItem
	{
		{get_local_language("Off"), [this]() { optionTouchCtrl = 0; emuViewController.setOnScreenControls(0); }},
		{get_local_language("On"), [this]() { optionTouchCtrl = 1; emuViewController.setOnScreenControls(1); }},
		{get_local_language("Auto"), [this]() { optionTouchCtrl = 2; emuViewController.updateAutoOnScreenControlVisible(); }}
	},
	touchCtrl
	{
		get_local_language("Use Virtual Gamepad"),
		optionTouchCtrl,
		touchCtrlItem
	},
	pointerInputItem
	{
		{"1", [this](){ setPointerInputPlayer(0); }},
		{"2", [this](){ setPointerInputPlayer(1); }},
		{"3", [this](){ setPointerInputPlayer(2); }},
		{"4", [this](){ setPointerInputPlayer(3); }},
		{"5", [this](){ setPointerInputPlayer(4); }}
	},
	pointerInput
	{
		get_local_language("Virtual Gamepad Player"),
		(int)pointerInputPlayer,
		[this](const MultiChoiceMenuItem &) -> int
		{
			return EmuSystem::maxPlayers;
		},
		[this](const MultiChoiceMenuItem &, uint idx) -> TextMenuItem&
		{
			return pointerInputItem[idx];
		}
	},
	sizeItem
	{
		{touchCtrlSizeMenuName[0], [this](){ setSize(touchCtrlSizeMenuVal[0]); }},
		{touchCtrlSizeMenuName[1], [this](){ setSize(touchCtrlSizeMenuVal[1]); }},
		{touchCtrlSizeMenuName[2], [this](){ setSize(touchCtrlSizeMenuVal[2]); }},
		{touchCtrlSizeMenuName[3], [this](){ setSize(touchCtrlSizeMenuVal[3]); }},
		{touchCtrlSizeMenuName[4], [this](){ setSize(touchCtrlSizeMenuVal[4]); }},
		{touchCtrlSizeMenuName[5], [this](){ setSize(touchCtrlSizeMenuVal[5]); }},
		{touchCtrlSizeMenuName[6], [this](){ setSize(touchCtrlSizeMenuVal[6]); }},
		{touchCtrlSizeMenuName[7], [this](){ setSize(touchCtrlSizeMenuVal[7]); }},
		{touchCtrlSizeMenuName[8], [this](){ setSize(touchCtrlSizeMenuVal[8]); }},
		{touchCtrlSizeMenuName[9], [this](){ setSize(touchCtrlSizeMenuVal[9]); }},
		{get_local_language("Custom Value"),
			[this](Input::Event e)
			{
				EmuApp::pushAndShowNewCollectValueInputView<double>(attachParams(), e, get_local_language("Input 3.0 to 15.0"), "",
					[this](auto val)
					{
						int scaledIntVal = val * 100.0;
						if(optionTouchCtrlSize.isValidVal(scaledIntVal))
						{
							setSize(scaledIntVal);
							size.setSelected(std::size(sizeItem) - 1, *this);
							popAndShow();
							return true;
						}
						else
						{
							EmuApp::postErrorMessage(get_local_language("Value not in range"));
							return false;
						}
					});
				return false;
			}
		},
	},
	size
	{
		get_local_language("Button Size"),
		[this](uint32_t idx)
		{
			return sizeStr;
		},
		findIdxInArrayOrDefault(touchCtrlSizeMenuVal, (uint)optionTouchCtrlSize, std::size(sizeItem) - 1),
		sizeItem
	},
	deadzoneItem
	{
		{touchDpadDeadzoneMenuName[0], [this](){ setDeadzone(touchDpadDeadzoneMenuVal[0]); }},
		{touchDpadDeadzoneMenuName[1], [this](){ setDeadzone(touchDpadDeadzoneMenuVal[1]); }},
		{touchDpadDeadzoneMenuName[2], [this](){ setDeadzone(touchDpadDeadzoneMenuVal[2]); }},
	},
	deadzone
	{
		get_local_language("Deadzone"),
		findIdxInArrayOrDefault(touchDpadDeadzoneMenuVal, (uint)optionTouchDpadDeadzone, 0),
		deadzoneItem
	},
	diagonalSensitivityItem
	{
		{touchDpadDiagonalSensitivityMenuName[0], [this](){ setDiagonalSensitivity(touchDpadDiagonalSensitivityMenuVal[0]); }},
		{touchDpadDiagonalSensitivityMenuName[1], [this](){ setDiagonalSensitivity(touchDpadDiagonalSensitivityMenuVal[1]); }},
		{touchDpadDiagonalSensitivityMenuName[2], [this](){ setDiagonalSensitivity(touchDpadDiagonalSensitivityMenuVal[2]); }},
		{touchDpadDiagonalSensitivityMenuName[3], [this](){ setDiagonalSensitivity(touchDpadDiagonalSensitivityMenuVal[3]); }},
		{touchDpadDiagonalSensitivityMenuName[4], [this](){ setDiagonalSensitivity(touchDpadDiagonalSensitivityMenuVal[4]); }},
	},
	diagonalSensitivity
	{
		get_local_language("Diagonal Sensitivity"),
		findIdxInArrayOrDefault(touchDpadDiagonalSensitivityMenuVal, (uint)optionTouchDpadDiagonalSensitivity, 0),
		diagonalSensitivityItem
	},
	btnSpaceItem
	{
		{touchCtrlBtnSpaceMenuName[0], [this](){ setButtonSpace(touchCtrlBtnSpaceMenuVal[0]); }},
		{touchCtrlBtnSpaceMenuName[1], [this](){ setButtonSpace(touchCtrlBtnSpaceMenuVal[1]); }},
		{touchCtrlBtnSpaceMenuName[2], [this](){ setButtonSpace(touchCtrlBtnSpaceMenuVal[2]); }},
		{touchCtrlBtnSpaceMenuName[3], [this](){ setButtonSpace(touchCtrlBtnSpaceMenuVal[3]); }},
	},
	btnSpace
	{
		get_local_language("Spacing"),
		findIdxInArrayOrDefault(touchCtrlBtnSpaceMenuVal, (uint)optionTouchCtrlBtnSpace, 0),
		btnSpaceItem
	},
	btnExtraXSizeItem
	{
		{touchCtrlExtraXBtnSizeMenuName[0], [this](){ setButtonExtraXSize(touchCtrlExtraXBtnSizeMenuVal[0]); }},
		{touchCtrlExtraXBtnSizeMenuName[1], [this](){ setButtonExtraXSize(touchCtrlExtraXBtnSizeMenuVal[1]); }},
		{touchCtrlExtraXBtnSizeMenuName[2], [this](){ setButtonExtraXSize(touchCtrlExtraXBtnSizeMenuVal[2]); }},
		{touchCtrlExtraXBtnSizeMenuName[3], [this](){ setButtonExtraXSize(touchCtrlExtraXBtnSizeMenuVal[3]); }},
	},
	btnExtraXSize
	{
		get_local_language("H Overlap"),
		findIdxInArrayOrDefault(touchCtrlExtraXBtnSizeMenuVal, (uint)optionTouchCtrlExtraXBtnSize, 0),
		btnExtraXSizeItem
	},
	btnExtraYSizeMultiRowItem
	{
		// uses same values as X counter-part
		{touchCtrlExtraXBtnSizeMenuName[0], [this](){ setButtonExtraYSizeMultiRow(touchCtrlExtraXBtnSizeMenuVal[0]); }},
		{touchCtrlExtraXBtnSizeMenuName[1], [this](){ setButtonExtraYSizeMultiRow(touchCtrlExtraXBtnSizeMenuVal[1]); }},
		{touchCtrlExtraXBtnSizeMenuName[2], [this](){ setButtonExtraYSizeMultiRow(touchCtrlExtraXBtnSizeMenuVal[2]); }},
		{touchCtrlExtraXBtnSizeMenuName[3], [this](){ setButtonExtraYSizeMultiRow(touchCtrlExtraXBtnSizeMenuVal[3]); }},
	},
	btnExtraYSizeMultiRow
	{
		(EmuSystem::inputFaceBtns == 4 || (EmuSystem::inputFaceBtns >= 6 && EmuSystem::inputHasTriggerBtns))
			? get_local_language("V Overlap") : get_local_language("V Overlap (2 rows)"),
		findIdxInArrayOrDefault(touchCtrlExtraXBtnSizeMenuVal, (uint)optionTouchCtrlExtraYBtnSizeMultiRow, 0),
		btnExtraYSizeMultiRowItem
	},
	btnExtraYSizeItem
	{
		{touchCtrlExtraYBtnSizeMenuName[0], [this](){ setButtonExtraYSize(touchCtrlExtraYBtnSizeMenuVal[0]); }},
		{touchCtrlExtraYBtnSizeMenuName[1], [this](){ setButtonExtraYSize(touchCtrlExtraYBtnSizeMenuVal[1]); }},
		{touchCtrlExtraYBtnSizeMenuName[2], [this](){ setButtonExtraYSize(touchCtrlExtraYBtnSizeMenuVal[2]); }},
		{touchCtrlExtraYBtnSizeMenuName[3], [this](){ setButtonExtraYSize(touchCtrlExtraYBtnSizeMenuVal[3]); }},
	},
	btnExtraYSize
	{
		get_local_language("V Overlap"),
		findIdxInArrayOrDefault(touchCtrlExtraYBtnSizeMenuVal, (uint)optionTouchCtrlExtraYBtnSize, 0),
		btnExtraYSizeItem
	},
	triggerPos
	{
		get_local_language("Inline L/R"),
		(bool)optionTouchCtrlTriggerBtnPos,
		[this](BoolMenuItem &item, Input::Event e)
		{
			optionTouchCtrlTriggerBtnPos = item.flipBoolValue(*this);
			EmuControls::setupVControllerVars();
		}
	},
	btnStaggerItem
	{
		{"-0.75x V", [this](){ setButtonStagger(0); }},
		{"-0.5x V", [this](){ setButtonStagger(1); }},
		{"0", [this](){ setButtonStagger(2); }},
		{"0.5x V", [this](){ setButtonStagger(3); }},
		{"0.75x V", [this](){ setButtonStagger(4); }},
		{"1x H&V", [this](){ setButtonStagger(5); }},
	},
	btnStagger
	{
		get_local_language("Stagger"),
		(int)optionTouchCtrlBtnStagger,
		btnStaggerItem
	},
	dPadStateItem
	{
		{ctrlStateStr[0], [this](){ setButtonState(0, 0, window()); }},
		{ctrlStateStr[1], [this](){ setButtonState(1, 0, window()); }},
		{ctrlStateStr[2], [this](){ setButtonState(2, 0, window()); }},
	},
	dPadState
	{
		get_local_language("D-Pad"),
		(int)layoutPosArr(window())[0].state,
		dPadStateItem
	},
	faceBtnStateItem
	{
		{ctrlStateStr[0], [this](){ setButtonState(0, 2, window()); }},
		{ctrlStateStr[1], [this](){ setButtonState(1, 2, window()); }},
		{ctrlStateStr[2], [this](){ setButtonState(2, 2, window()); }},
	},
	faceBtnState
	{
		faceBtnName,
		(int)layoutPosArr(window())[2].state,
		faceBtnStateItem
	},
	centerBtnStateItem
	{
		{ctrlStateStr[0], [this](){ setButtonState(0, 1, window()); }},
		{ctrlStateStr[1], [this](){ setButtonState(1, 1, window()); }},
		{ctrlStateStr[2], [this](){ setButtonState(2, 1, window()); }},
	},
	centerBtnState
	{
		centerBtnName,
		(int)layoutPosArr(window())[1].state,
		centerBtnStateItem
	},
	lTriggerStateItem
	{
		{ctrlStateStr[0], [this](){ setButtonState(0, 5, window()); }},
		{ctrlStateStr[1], [this](){ setButtonState(1, 5, window()); }},
		{ctrlStateStr[2], [this](){ setButtonState(2, 5, window()); }},
	},
	lTriggerState
	{
		"L",
		(int)layoutPosArr(window())[5].state,
		lTriggerStateItem
	},
	rTriggerStateItem
	{
		{ctrlStateStr[0], [this](){ setButtonState(0, 6, window()); }},
		{ctrlStateStr[1], [this](){ setButtonState(1, 6, window()); }},
		{ctrlStateStr[2], [this](){ setButtonState(2, 6, window()); }},
	},
	rTriggerState
	{
		"R",
		(int)layoutPosArr(window())[6].state,
		rTriggerStateItem
	},
	boundingBoxes
	{
		get_local_language("Show Bounding Boxes"),
		(bool)optionTouchCtrlBoundingBoxes,
		[this](BoolMenuItem &item, Input::Event e)
		{
			optionTouchCtrlBoundingBoxes = item.flipBoolValue(*this);
			EmuControls::setupVControllerVars();
			postDraw();
		}
	},
	vibrate
	{
		get_local_language("Vibration"),
		(bool)optionVibrateOnPush,
		[this](BoolMenuItem &item, Input::Event e)
		{
			optionVibrateOnPush = item.flipBoolValue(*this);
		}
	},
		#ifdef CONFIG_BASE_ANDROID
		useScaledCoordinates
		{
			get_local_language("Size Units"),
			(bool)optionTouchCtrlScaledCoordinates,
			get_local_language("Physical (Millimeters)"), get_local_language("Scaled Points"),
			[this](BoolMenuItem &item, Input::Event e)
			{
				optionTouchCtrlScaledCoordinates = item.flipBoolValue(*this);
				EmuControls::setupVControllerVars();
				this->vController.place();
			}
		},
		#endif
	showOnTouch
	{
		get_local_language("Show Gamepad If Screen Touched"),
		(bool)optionTouchCtrlShowOnTouch,
		[this](BoolMenuItem &item, Input::Event e)
		{
			optionTouchCtrlShowOnTouch = item.flipBoolValue(*this);
		}
	},
	#endif // CONFIG_VCONTROLS_GAMEPAD
	alphaItem
	{
		{alphaMenuName[0], [this](){ setAlpha(alphaMenuVal[0]); }},
		{alphaMenuName[1], [this](){ setAlpha(alphaMenuVal[1]); }},
		{alphaMenuName[2], [this](){ setAlpha(alphaMenuVal[2]); }},
		{alphaMenuName[3], [this](){ setAlpha(alphaMenuVal[3]); }},
		{alphaMenuName[4], [this](){ setAlpha(alphaMenuVal[4]); }},
		{alphaMenuName[5], [this](){ setAlpha(alphaMenuVal[5]); }},
	},
	alpha
	{
		get_local_language("Blend Amount"),
		findIdxInArrayOrDefault(alphaMenuVal, optionTouchCtrlAlpha.val, 3),
		alphaItem
	},
	btnPlace
	{
		get_local_language("Set Button Positions"),
		[this](Input::Event e)
		{
			auto onScreenInputPlace = makeView<OnScreenInputPlaceView>();
			onScreenInputPlace->init(); // TODO: rework view init
			emuViewController.pushAndShowModal(std::move(onScreenInputPlace), e, false);
		}
	},
	menuStateItem
	{
		{ctrlStateStr[0], [this](){ setButtonState(0, 3, window()); }},
		{ctrlStateStr[1], [this](){ setButtonState(1, 3, window()); }},
		{ctrlStateStr[2], [this](){ setButtonState(2, 3, window()); }},
	},
	menuState
	{
		get_local_language("Open Menu Button"),
		(int)layoutPosArr(window())[3].state,
		[](const MultiChoiceMenuItem &) -> int
		{
			return CAN_TURN_OFF_MENU_BTN ? 3 : 2; // iOS port doesn't use "off" value
		},
		[this](const MultiChoiceMenuItem &, uint idx) -> TextMenuItem&
		{
			return menuStateItem[CAN_TURN_OFF_MENU_BTN ? idx : idx + 1];
		}
	},
	ffStateItem
	{
		{ctrlStateStr[0], [this](){ setButtonState(0, 4, window()); }},
		{ctrlStateStr[1], [this](){ setButtonState(1, 4, window()); }},
		{ctrlStateStr[2], [this](){ setButtonState(2, 4, window()); }},
	},
	ffState
	{
		get_local_language("Fast-forward Button"),
		(int)layoutPosArr(window())[4].state,
		ffStateItem
	},
	resetControls
	{
		get_local_language("Reset Position & Spacing Options"),
		[this](Input::Event e)
		{
			auto ynAlertView = makeView<YesNoAlertView>(get_local_language("Reset buttons to default positions & spacing?"));
			ynAlertView->setOnYes(
				[this](TextMenuItem &, View &view, Input::Event e)
				{
					view.dismiss();
					resetVControllerOptions();
					EmuControls::setupVControllerVars();
					refreshTouchConfigMenu();
				});
			emuViewController.pushAndShowModal(std::move(ynAlertView), e, false);
		}
	},
	resetAllControls
	{
		get_local_language("Reset All Options"),
		[this](Input::Event e)
		{
			auto ynAlertView = makeView<YesNoAlertView>(get_local_language("Reset all on-screen control options to default?"));
			ynAlertView->setOnYes(
				[this](TextMenuItem &, View &view, Input::Event e)
				{
					view.dismiss();
					resetAllVControllerOptions();
					EmuControls::setupVControllerVars();
					refreshTouchConfigMenu();
				});
			emuViewController.pushAndShowModal(std::move(ynAlertView), e, false);
		}
	},
	btnTogglesHeading
	{
		get_local_language("Individual Button Toggles")
	},
	dpadtHeading
	{
		get_local_language("D-Pad Options")
	},
	faceBtnHeading
	{
		get_local_language("Face Button Layout")
	},
	otherHeading
	{
		get_local_language("Other Options")
	}
{
	#ifdef CONFIG_VCONTROLS_GAMEPAD
	item.emplace_back(&touchCtrl);
	if(EmuSystem::maxPlayers > 1)
	{
		item.emplace_back(&pointerInput);
	}
	string_printf(sizeStr, "%.2f", optionTouchCtrlSize / 100.);
	item.emplace_back(&size);
	#endif
	item.emplace_back(&btnPlace);
	item.emplace_back(&btnTogglesHeading);
	auto &layoutPos = layoutPosArr(window());
	{
		if(!CAN_TURN_OFF_MENU_BTN) // prevent iOS port from disabling menu control
		{
			if(layoutPos[3].state == 0)
				layoutPos[3].state = 1;
		}
		item.emplace_back(&menuState);
	}
	item.emplace_back(&ffState);
	#ifdef CONFIG_VCONTROLS_GAMEPAD
	item.emplace_back(&dPadState);
	item.emplace_back(&faceBtnState);
	item.emplace_back(&centerBtnState);
	if(vController.hasTriggers())
	{
		item.emplace_back(&lTriggerState);
		item.emplace_back(&rTriggerState);
		item.emplace_back(&triggerPos);
	}
	item.emplace_back(&dpadtHeading);
	item.emplace_back(&deadzone);
	item.emplace_back(&diagonalSensitivity);
	item.emplace_back(&faceBtnHeading);
	item.emplace_back(&btnSpace);
	item.emplace_back(&btnStagger);
	item.emplace_back(&btnExtraXSize);
	if(EmuSystem::inputFaceBtns < 4 || (EmuSystem::inputFaceBtns == 6 && !EmuSystem::inputHasTriggerBtns))
	{
		item.emplace_back(&btnExtraYSize);
	}
	if(EmuSystem::inputFaceBtns >= 4)
	{
		item.emplace_back(&btnExtraYSizeMultiRow);
	}
	item.emplace_back(&otherHeading);
	item.emplace_back(&boundingBoxes);
	if(!optionVibrateOnPush.isConst)
	{
		item.emplace_back(&vibrate);
	}
	item.emplace_back(&showOnTouch);
		#ifdef CONFIG_BASE_ANDROID
		item.emplace_back(&useScaledCoordinates);
		#endif
	#else
	item.emplace_back(&otherHeading);
	#endif // CONFIG_VCONTROLS_GAMEPAD
	item.emplace_back(&alpha);
	item.emplace_back(&resetControls);
	item.emplace_back(&resetAllControls);
}
