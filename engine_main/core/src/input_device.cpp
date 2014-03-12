#include "input_device.h"
#include "app.h"
#include "input_event.h"
#include "input_manager.h"

SimpleButton::SimpleButton()
	: m_state(0)
	, m_value(0)
{
	
}

bool SimpleButton::is_down() const
{
	return (m_state & IS_DOWN_MASK) != 0;
}

bool SimpleButton::is_up() const 
{
	return !is_down(); 
}

// Up count logic:
// | Action     | A= DownCount | B= IsDown | C= WasDown | UpCount = A - B + C |
// ----------------------------------------------------------------------------
// | New Frame  |            0 |         0 |          0 |                   0 |
// | Touch Down |            1 |         1 |          0 |                   0 |
// | Touch Up   |            1 |         0 |          0 |                   1 |
// | Touch Down |            2 |         1 |          0 |                   1 |
// | Touch Up   |            2 |         0 |          0 |                   2 |
// | Touch Down |            3 |         1 |          0 |                   2 |
// ----------------------------------------------------------------------------
// | New Frame  |            0 |         1 |          1 |                   0 |
// | Touch Up   |            0 |         0 |          1 |                   1 |
// | Touch Down |            1 |         1 |          1 |                   1 |
// | Touch Up   |            1 |         0 |          1 |                   2 |
// ----------------------------------------------------------------------------
// | New Frame  |            0 |         0 |          0 |                   0 |
// | Touch Down |            1 |         1 |          0 |                   0 |
// | Touch Up   |            1 |         0 |          0 |                   1 |
// ...

int SimpleButton::get_up_count() const 
{
	return get_down_count() - is_down() + was_down();  
}

int SimpleButton::get_down_count() const 
{
	return (m_state & DOWN_COUNT_MASK);
}

unsigned short SimpleButton::get_value() const 
{
	return m_value; 
}

void SimpleButton::set_value(unsigned short val)
{
	m_value = val; 
} 

bool SimpleButton::was_pressed() const
{
	return get_down_count() && was_up(); 
}

bool SimpleButton::was_released() const
{
	return get_up_count() && was_down();
}

void SimpleButton::reset_frame()
{
	m_state = is_down() ? (IS_DOWN_MASK | WAS_DOWN_MASK) : 0; 
}

void SimpleButton::update_frame(bool is_pressed)
{
	if (is_pressed != is_down())
	{
		if (is_pressed) 
		{
			m_state |= IS_DOWN_MASK; 
			m_state++; 
		}
		else 
			m_state &= ~IS_DOWN_MASK;
	}
}

bool SimpleButton::was_down() const
{
	return (m_state & WAS_DOWN_MASK) > 0; 
}

bool SimpleButton::was_up() const
{
	return !was_down(); 
}

////////////////////////////////////////////////////////////////////////// 

TouchPad::Touch::Touch()
	: m_touch_id(UNDEFINED_TOUCH_ID)
{
	
}

TouchPad::TouchPad()
	: m_num_active_touches(0)
{
	//mActiveTouchIndexes contains touch indexes for: 
	// - all active touches in the range [0, mActiveTouchCount[
	// - all inactive touches in the range [mActiveTouchCount, NUM_TOUCHES[
	for (int i = 0; i < (int)NUM_TOUCHES; i++)
		m_active_touches_idx[i] = i; 
}

void TouchPad::raise_touch_event(InputDevice* device, 
	int touch_pad_idx, 
	InputEventTypes event, 
	uint touch_id, 
	float x, float y)
{
	TouchPadEvent tp_evt(event); 
	tp_evt.m_input_device = device; 
	tp_evt.m_touchpad_idx = touch_pad_idx; 
	tp_evt.m_touch_id = touch_id; 
	//tp_evt.m_point = App::GetInstance()->convert_pos_device_to_screen(Point<float>(x, y)); 
	tp_evt.m_point = Point<short>(short(x), short(y));
	
	get_app()->get_event_mgr()->post_event(tp_evt); 
} 

bool TouchPad::update(InputEventTypes evt_type, 
	unsigned int touch_id, 
	short x, 
	short y)
{
	if (evt_type == IET_TOUCHPAD_BEGAN)
	{
		// Get free touch	
		if (m_num_active_touches < (int)NUM_TOUCHES)
		{
			Touch &touch = m_touches[m_active_touches_idx[m_num_active_touches]];
			touch.m_touch_id = touch_id;
			touch.m_first = Point<short>(x, y); 
			touch.m_points.clear(); 
			++m_num_active_touches; 
			return true; 
		}
	}
	else 
	{
		// find the correspoinding touch
		for (int i = 0; i < m_num_active_touches; ++i)
		{
			const int touch_idx = m_active_touches_idx[i]; 
			Touch& touch = m_touches[touch_idx];
			if (touch.m_touch_id == touch_id)
			{
				touch.m_points.push_back(Point<short>(x, y)); 
				if (evt_type == IET_TOUCHPAD_ENDED)
				{
					// swap
					m_active_touches_idx[i] = m_active_touches_idx[m_num_active_touches-1]; 
					m_active_touches_idx[m_num_active_touches-1] = touch_idx; 
					--m_num_active_touches;
				}

				return true; 
			}
		}

		return true; 
	}

	return false;
}

////////////////////////////////////////////////////////////////////////// 

Cursor::Cursor()
	: m_last_pos(0, 0)
	, m_pos(0, 0)
{ 
}

bool Cursor::has_moved() const 
{
	Point<short> delta = m_pos - m_last_pos; 
	return delta.x != 0 || delta.y != 0; 
}

void Cursor::update_frame(bool is_visible, short x, short y)
{
	m_pos.Set(x, y); 
}


////////////////////////////////////////////////////////////////////////// 

InputDevice::InputDevice()
{
	
}

InputDevice::~InputDevice() 
{
	
}

void InputDevice::reset_frame()
{
	// Reset all buttons
	for (unsigned int i = 0; i < m_btns.size(); ++i)
	{
		m_btns[i].reset_frame(); 
	} 
}

void InputDevice::set_updated()
{
	get_app()->get_input_mgr()->add_updated_device(this); 
}

void InputDevice::update_button(unsigned int idx, bool is_down)
{
	if (idx < m_btns.size())
	{
		m_btns[idx].update_frame(is_down); 
		set_updated();
	}
}

void InputDevice::update_cursor(uint idx, int x, int y)
{
	m_cursors[idx].update_frame(true, static_cast<short>(x), static_cast<short>(y));
	set_updated(); 
}

////////////////////////////////////////////////////////////////////////// 

Mouse::Mouse()
	: m_wheel(0.0f)
{
	// .Set(GLF_ASIZEOF(mButtons), mButtons);
	for (int i = 0; i < BTN_COUNT; ++i)
		m_btns.push_back(m_mouse_btns[i]);
	m_cursors.push_back(m_cursor); 
}

void Mouse::raise_move_event(float x, float y)
{
	MouseEvent evt(IET_MOUSE_MOVED);
	
	evt.m_input_device = this;
	evt.m_btn = Mouse::BTN_NONE;
	evt.m_pos = Point<short>((short)x, (short)y);
	
	get_app()->get_event_mgr()->post_event(evt); 
}

void Mouse::raise_btn_event(int idx, bool is_down)
{
	const Btn btn = static_cast<Btn>(idx);
	MouseEvent evt(is_down? IET_MOUSE_BTN_DOWN : IET_MOUSE_BTN_UP);
	evt.m_btn = btn;
	evt.m_input_device = this; 
	
	get_app()->get_event_mgr()->post_event(evt);
}

bool Mouse::presend_event(CoreEvent& event)
{
	switch(event.type)
	{
	case IET_MOUSE_BTN_DOWN: 
	case IET_MOUSE_BTN_UP:
		{
			MouseEvent &evt = static_cast<MouseEvent&>(event); 
			// Update button state
			update_button(evt.m_btn, evt.type == IET_MOUSE_BTN_DOWN); 
			// Update mouse event pos with current state 
			evt.m_pos = get_pos();
			return true; 
		}
		break;

	case IET_MOUSE_MOVED: 
		{
			MouseEvent &evt = static_cast<MouseEvent&>(event); 
			// Update cursor state 
			update_cursor(0, evt.m_pos.x, evt.m_pos.y); 
			return true; 
		}
		break;

	case IET_MOUSE_WHEEL:
		break; 
	}
	
	return InputDevice::presend_event(event); 
}

void Mouse::reset_frame()
{
	InputDevice::reset_frame();
	
	// reset wheel
	m_wheel = 0.0f; 
}

void Mouse::update_wheel(float wheel)
{
	m_wheel += wheel; 	
}

////////////////////////////////////////////////////////////////////////// 

bool GamePad::presend_event(CoreEvent& evt)
{
	switch (evt.type)
	{
	case IET_TOUCHPAD_BEGAN:
	case IET_TOUCHPAD_MOVED:
	case IET_TOUCHPAD_ENDED:
		{
			TouchPadEvent& tp_evt = static_cast<TouchPadEvent&>(evt);
			
			// Update touchpad state 
			return m_touchpad.update((InputEventTypes)tp_evt.type, 
				tp_evt.m_touch_id, 
				tp_evt.m_point.x, 
				tp_evt.m_point.y); 
		}
		
	}

	return true;
}

void GamePad::raise_button_event(int idx, bool is_down)
{
	if (idx >= 0 && idx < (int)m_btns.size())
	{
		GamepadButtonEvent evt(is_down ? IET_GAMEPAD_BUTTON_PRESSED : IET_GAMEPAD_BUTTON_RELEASED);
		evt.m_input_device = this; 
		evt.m_btn_idx = idx; 
		get_app()->get_event_mgr()->post_event(evt); 
	}
}

void GamePad::raise_touch_event(InputEventTypes evt_type, uint touch_id, float x, float y)
{
	m_touchpad.raise_touch_event(this, 0, evt_type, touch_id, x, y);
}

////////////////////////////////////////////////////////////////////////// 

AndroidController::AndroidController()
{
	Cursor cursors[AndroidCursorsNum];
	m_ctrl_cursors = CursorsArray(cursors, cursors+AndroidCursorsNum);
	
	SimpleButton buttons[AndroidController::BTN_COUNT];
	m_ctrl_buttons = ButtonsArray(buttons, buttons+AndroidController::BTN_COUNT);
	
	m_cursors.swap(m_ctrl_cursors); 
	m_btns.swap(m_ctrl_buttons);
	
	m_touchpad = m_tpad; 
}