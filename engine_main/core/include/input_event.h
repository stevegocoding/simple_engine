#ifndef __INPUT_EVENT_H__
#define __INPUT_EVENT_H__

#include "config.h"
#include "core_event.h"
#include "types.h"
#include "input_device.h"

struct InputEvent : public CoreEvent
{
public:
	explicit InputEvent(int type)
		: CoreEvent(type) 
		, m_input_device(NULL)
		, m_timestamp(0)
	{
		
	}

	InputDevice *m_input_device;
	
	/** Time Stamp */
	int64 m_timestamp;
};


struct MouseEvent : public InputEvent
{
public:
	explicit MouseEvent(int event_type)
		: InputEvent(event_type)
		, m_btn(Mouse::BTN_NONE) 
	{ 
	}

	Mouse::Btn m_btn;
	Point<short> m_pos;
 
	// @TODO: wheel data 
};


struct TouchPadEvent : public InputEvent
{
public:
	explicit TouchPadEvent(int event_type)
		: InputEvent(event_type)
		, m_touch_id(0)
		, m_touchpad_idx(0)
	{ 
	}

	int m_touchpad_idx; 
	int m_touch_id; 
	Point<short> m_point; 
};


struct GamepadButtonEvent : public InputEvent
{
public:
	explicit GamepadButtonEvent(int event_type)
			: InputEvent(event_type)
			, m_btn_idx(-1)
	{
	}
	int m_btn_idx; 
};



#endif