/*  This file is part of Imagine.

	Imagine is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Imagine is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Imagine.  If not, see <http://www.gnu.org/licenses/> */

#include <imagine/input/Input.hh>
#include <imagine/base/Window.hh>
#include <imagine/base/Timer.hh>
#include <imagine/logger/logger.h>
#include <imagine/util/container/containerUtils.hh>
#include "private.hh"

namespace Input
{

static Base::Timer keyRepeatTimer{"keyRepeatTimer"};
static Event keyRepeatEvent{};
static bool allowKeyRepeats_ = true;
DeviceChangeDelegate onDeviceChange{};
DevicesEnumeratedDelegate onDevicesEnumerated{};
std::vector<Device*> devList{};
bool swappedGamepadConfirm_ = SWAPPED_GAMEPAD_CONFIRM_DEFAULT;
static uint32_t xPointerTransform_ = POINTER_NORMAL;
static uint32_t yPointerTransform_ = POINTER_NORMAL;
static uint32_t pointerAxis_ = POINTER_NORMAL;

static constexpr Key iCadeMap[12]
{
	Keycode::UP, Keycode::RIGHT, Keycode::DOWN, Keycode::LEFT,
	Keycode::GAME_X, Keycode::GAME_B,
	Keycode::GAME_A, Keycode::GAME_Y,
	Keycode::GAME_C, Keycode::GAME_Z,
	Keycode::GAME_START, Keycode::GAME_SELECT
};

const std::vector<Device*> &deviceList()
{
	return devList;
}

void setAllowKeyRepeats(bool on)
{
	allowKeyRepeats_ = on;
	if(!on)
	{
		deinitKeyRepeatTimer();
	}
}

bool allowKeyRepeats()
{
	return allowKeyRepeats_;
}

void startKeyRepeatTimer(Event event)
{
	if(!allowKeyRepeats_)
		return;
	if(!event.pushed())
	{
		// only repeat PUSHED action, otherwise cancel the timer
		//logMsg("repeat event is not for pushed action");
		cancelKeyRepeatTimer();
		return;
	}
	//logMsg("starting key repeat");
	keyRepeatEvent = event;
	keyRepeatEvent.setRepeatCount(1);
	keyRepeatTimer.callbackAfterMSec(
		[]()
		{
			logMsg("repeating key event");
			if(likely(keyRepeatEvent.pushed()))
				dispatchInputEvent(keyRepeatEvent);
		}, 400, 50, {}, Base::Timer::HINT_REUSE);
}

void cancelKeyRepeatTimer()
{
	//logMsg("cancelled key repeat");
	keyRepeatTimer.cancel();
	keyRepeatEvent = {};
}

void deinitKeyRepeatTimer()
{
	//logMsg("deinit key repeat");
	keyRepeatTimer.deinit();
	keyRepeatEvent = {};
}

void addDevice(Device &d)
{
	d.idx = devList.size();
	devList.emplace_back(&d);
}

void removeDevice(Device &d)
{
	logMsg("removing device: %s,%d", d.name(), d.enumId());
	cancelKeyRepeatTimer();
	IG::removeFirst(devList, &d);
	indexDevices();
}

void indexDevices()
{
	// re-number device indices
	uint32_t i = 0;
	for(auto &e : Input::devList)
	{
		e->idx = i;
		i++;
	}
}

void xPointerTransform(uint32_t mode)
{
	xPointerTransform_ = mode;
}

void yPointerTransform(uint32_t mode)
{
	yPointerTransform_ = mode;
}

void pointerAxis(uint32_t mode)
{
	pointerAxis_ = mode;
}

IG::Point2D<int> transformInputPos(const Base::Window &win, IG::Point2D<int> srcPos)
{
	IG::Point2D<int> pos;
	// x,y axis is swapped first
	pos.x = pointerAxis_ == POINTER_INVERT ? srcPos.y : srcPos.x;
	pos.y = pointerAxis_ == POINTER_INVERT ? srcPos.x : srcPos.y;

	// then coordinates are inverted
	if(xPointerTransform_ == POINTER_INVERT)
		pos.x = win.width() - pos.x;
	if(yPointerTransform_ == POINTER_INVERT)
		pos.y = win.height() - pos.y;
	return pos;
}

// For soft-orientation
void configureInputForOrientation(const Base::Window &win)
{
	using namespace Input;
	using namespace Base;
	xPointerTransform(win.softOrientation() == VIEW_ROTATE_0 || win.softOrientation() == VIEW_ROTATE_90 ? POINTER_NORMAL : POINTER_INVERT);
	yPointerTransform(win.softOrientation() == VIEW_ROTATE_0 || win.softOrientation() == VIEW_ROTATE_270 ? POINTER_NORMAL : POINTER_INVERT);
	pointerAxis(win.softOrientation() == VIEW_ROTATE_0 || win.softOrientation() == VIEW_ROTATE_180 ? POINTER_NORMAL : POINTER_INVERT);
}

bool keyInputIsPresent()
{
	return Device::anyTypeBitsPresent(Device::TYPE_BIT_KEYBOARD | Device::TYPE_BIT_GAMEPAD);
}

bool dispatchInputEvent(Input::Event event)
{
	return Base::mainWindow().dispatchInputEvent(event);
}

static Key keyToICadeOnKey(Key key)
{
	switch(key)
	{
		case Keycode::W : return iCadeMap[0];
		case Keycode::D : return iCadeMap[1];
		case Keycode::X : return iCadeMap[2];
		case Keycode::A : return iCadeMap[3];
		case Keycode::Y : return iCadeMap[4];
		case Keycode::H : return iCadeMap[5];
		case Keycode::U : return iCadeMap[6];
		case Keycode::J : return iCadeMap[7];
		case Keycode::I : return iCadeMap[8];
		case Keycode::K : return iCadeMap[9];
		case Keycode::O : return iCadeMap[10];
		case Keycode::L : return iCadeMap[11];
	}
	return 0;
}

static Key keyToICadeOffKey(Key key)
{
	switch(key)
	{
		case Keycode::E : return iCadeMap[0];
		case Keycode::C : return iCadeMap[1];
		case Keycode::Z : return iCadeMap[2];
		case Keycode::Q : return iCadeMap[3];
		case Keycode::T : return iCadeMap[4];
		case Keycode::R : return iCadeMap[5];
		case Keycode::F : return iCadeMap[6];
		case Keycode::N : return iCadeMap[7];
		case Keycode::M : return iCadeMap[8];
		case Keycode::P : return iCadeMap[9];
		case Keycode::G : return iCadeMap[10];
		case Keycode::V : return iCadeMap[11];
	}
	return 0;
}

bool processICadeKey(Key key, uint32_t action, Time time, const Device &dev, Base::Window &win)
{
	if(Key onKey = keyToICadeOnKey(key))
	{
		if(action == PUSHED)
		{
			//logMsg("pushed iCade keyboard key: %s", dev.keyName(key));
			Event event{0, Event::MAP_ICADE, onKey, onKey, PUSHED, 0, 0, time, &dev};
			startKeyRepeatTimer(event);
			win.dispatchInputEvent(event);
		}
		return true;
	}
	if(Key offKey = keyToICadeOffKey(key))
	{
		if(action == PUSHED)
		{
			cancelKeyRepeatTimer();
			win.dispatchInputEvent({0, Event::MAP_ICADE, offKey, offKey, RELEASED, 0, 0, time, &dev});
		}
		return true;
	}
	return false; // not an iCade key
}

void setOnDeviceChange(DeviceChangeDelegate del)
{
	onDeviceChange = del;
}

void setOnDevicesEnumerated(DevicesEnumeratedDelegate del)
{
	onDevicesEnumerated = del;
}

const char *Event::mapName(uint32_t map)
{
	switch(map)
	{
		case MAP_NULL: return "Null";
		case MAP_SYSTEM: return "Key Input";
		case MAP_POINTER: return "Pointer";
		case MAP_REL_POINTER: return "Relative Pointer";
		#ifdef CONFIG_BLUETOOTH
		case MAP_WIIMOTE: return "Wiimote";
		case MAP_WII_CC: return "Classic / Wii U Pro Controller";
		case MAP_ICONTROLPAD: return "iControlPad";
		case MAP_ZEEMOTE: return "Zeemote JS1";
		#endif
		#ifdef CONFIG_BLUETOOTH_SERVER
		case MAP_PS3PAD: return "PS3 Gamepad";
		#endif
		case MAP_ICADE: return "iCade";
		#ifdef CONFIG_INPUT_APPLE_GAME_CONTROLLER
		case MAP_APPLE_GAME_CONTROLLER: return "Apple Game Controller";
		#endif
		default: return "Unknown";
	}
}

uint32_t Event::mapNumKeys(uint32_t map)
{
	switch(map)
	{
		case MAP_NULL: return 0;
		case MAP_SYSTEM: return Input::Keycode::COUNT;
		#ifdef CONFIG_BLUETOOTH
		case MAP_WIIMOTE: return Input::Wiimote::COUNT;
		case MAP_WII_CC: return Input::WiiCC::COUNT;
		case MAP_ICONTROLPAD: return Input::iControlPad::COUNT;
		case MAP_ZEEMOTE: return Input::Zeemote::COUNT;
		#endif
		#ifdef CONFIG_BLUETOOTH_SERVER
		case MAP_PS3PAD: return Input::PS3::COUNT;
		#endif
		case MAP_ICADE: return Input::ICade::COUNT;
		#ifdef CONFIG_INPUT_APPLE_GAME_CONTROLLER
		case MAP_APPLE_GAME_CONTROLLER: return Input::AppleGC::COUNT;
		#endif
		default: bug_unreachable("map == %d", map); return 0;
	}
}

Event defaultEvent()
{
	Event e{};
	e.setMap(keyInputIsPresent() ? Event::MAP_SYSTEM : Event::MAP_POINTER);
	return e;
}

#ifndef CONFIG_INPUT_SYSTEM_COLLECTS_TEXT
uint32_t startSysTextInput(InputTextDelegate callback, const char *initialText, const char *promptText, uint32_t fontSizePixels)
{
	return 0;
}

void cancelSysTextInput() {}

void finishSysTextInput() {}

void placeSysTextInput(IG::WindowRect rect) {}

IG::WindowRect sysTextInputRect()
{
	return {};
}
#endif

void setSwappedGamepadConfirm(bool swapped)
{
	swappedGamepadConfirm_ = swapped;
}

bool swappedGamepadConfirm()
{
	return swappedGamepadConfirm_;
}

}
