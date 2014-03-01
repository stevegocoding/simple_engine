#include "input_manager_win32.h"

InputManagerWin32::InputManagerWin32()
{
	
}

InputManagerWin32::~InputManagerWin32()
{
	
}

void InputManagerWin32::preupdate_frame()
{
	m_input.pre_update_frame(); 
}

void InputManagerWin32::update_frame() 
{
	InputManager::update_frame();
}


void InputManagerWin32::on_wnd_events(NativeWndMsg& wnd_msg)
{
	Win32NativeWndMsg& win32_msg = static_cast<Win32NativeWndMsg&>(wnd_msg);
	int x_pos = GET_X_LPARAM(win32_msg.lParam); 
	int y_pos = GET_Y_LPARAM(win32_msg.lParam);

	switch(win32_msg.msg)
	{
	case WM_MOUSEMOVE: 
		{ 		
			m_input.m_mouse.raise_move_event((float)x_pos, (float)y_pos);
			m_input.touch_move(x_pos, y_pos); 
		}
		break; 
		
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP: 
		{	
			// Mouse Button Event
			m_input.m_mouse.raise_btn_event(Mouse::BTN_LEFT, win32_msg.msg==WM_LBUTTONDOWN);
			
			// TouchPad Emulation 
			if (win32_msg.msg == WM_LBUTTONDOWN)
			{
				m_input.touch_begin(x_pos, y_pos); 
			}
			else if (win32_msg.msg == WM_LBUTTONUP)
			{
				m_input.touch_end(x_pos, y_pos); 
			}
		}
		break;  
	} 
}

////////////////////////////////////////////////////////////////////////// 

void InputManagerWin32::InputInfo::pre_update_frame()
{
	
}

void InputManagerWin32::InputInfo::update_frame()
{	
}

void InputManagerWin32::InputInfo::touch_begin(int x, int y)
{
	m_gamepad.raise_touch_event(IET_TOUCHPAD_BEGAN, m_current_touch_id, (float)x, (float)y); 
	m_touch_started = true; 
}

void InputManagerWin32::InputInfo::touch_move(int x, int y)
{
	if (!m_touch_started)
		return; 
	
	m_gamepad.raise_touch_event(IET_TOUCHPAD_MOVED, m_current_touch_id, (float)x, (float)y); 
}

void InputManagerWin32::InputInfo::touch_end(int x, int y, int touch_id /* = -1 */)
{
	m_gamepad.raise_touch_event(IET_TOUCHPAD_ENDED, m_current_touch_id, (float)x, (float)y); 
	m_touch_started = false;
}