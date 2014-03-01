#ifndef __INPUT_MANAGER_WIN32_H__
#define __INPUT_MANAGER_WIN32_H__

#include <windows.h>
#include <windowsx.h>
#include "input_manager.h"
#include "input_device.h"


struct Win32NativeWndMsg : public NativeWndMsg
{
	UINT msg; 
	LPARAM lParam;
	WPARAM wParam; 
	
	Win32NativeWndMsg(UINT _msg, LPARAM _lParam, WPARAM _wParam)
		: msg(_msg)
		, lParam(_lParam)
		, wParam(_wParam)
	{}
};


class InputManagerWin32 : public InputManager
{
public:
	InputManagerWin32(); 
	~InputManagerWin32();

	virtual void preupdate_frame(); 
	virtual void update_frame(); 

	virtual void on_wnd_events(NativeWndMsg& wnd_msg); 

private:
	
	struct InputInfo
	{
		InputInfo()
			: m_current_touch_id(0)
			, m_touch_started(false)
		{ 	
		}

		void pre_update_frame();
		void update_frame();

		void touch_begin(int x, int y);
		void touch_move(int x, int y);
		void touch_end(int x, int y, int touch_id = -1); 
		
		Mouse m_mouse; 
		GamePad m_gamepad;

		unsigned int m_current_touch_id; 
		bool m_touch_started;
	}; 


	InputInfo m_input;
};


#endif
