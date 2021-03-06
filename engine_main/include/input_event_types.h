#ifndef __INPUT_EVENT_TYPES__
#define __INPUT_EVENT_TYPES__

#include "core_event.h"

enum InputEventTypes
{
	// Used by MouseEvent
	IET_INPUT_START = INPUT_EVENT_TYPE_BASE,
	IET_MOUSE_BTN_DOWN = IET_INPUT_START,
	IET_MOUSE_BTN_UP,
	IET_MOUSE_MOVED,
	IET_MOUSE_WHEEL,		
	// Used by KeyboardEvent
	IET_KEY_PRESSED,
	IET_KEY_RELEASED,		
	// used when gamepad is connected/disconnected
	IET_GAMEPAD_CONNECTION,
	// Used by GamepadButtonEvent
	IET_GAMEPAD_BUTTON_PRESSED,
	IET_GAMEPAD_BUTTON_RELEASED,		
	// Used by GamepadCursor
	IET_GAMEPAD_CURSOR_IN,
	IET_GAMEPAD_CURSOR_MOVED,
	IET_GAMEPAD_CURSOR_OUT,


	// Used by GamepadSensorEvent
	IET_GAMEPAD_SENSOR_STICK_CHANGED,
	IET_GAMEPAD_SENSOR_ACCELEROMETER_CHANGED,
	IET_GAMEPAD_SENSOR_GYROSCOPE_CHANGED,		
	// Used by TouchPadEvent
	IET_TOUCHPAD_BEGAN,
	IET_TOUCHPAD_MOVED,
	IET_TOUCHPAD_ENDED,

	IET_INPUT_END,
};

#endif 