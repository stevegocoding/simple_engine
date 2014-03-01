#ifndef __WINDOW_EVENT_UTILS_H__
#define __WINDOW_EVENT_UTILS_H__

#include "prerequisites.h"

#if SIMPLE_ENGINE_PLATFORM == SE_PLATFORM_WIN32
	#if !defined(WIN32_LEAN_AND_MEAN)
		#define WIN32_LEAN_AND_MEAN
	#endif

	#include <windows.h>
#endif 

class WindowEventUtils 
{
	
public:
	
	static void dispatch_message(); 

#if SIMPLE_ENGINE_PLATFORM == SE_PLATFORM_WIN32
	static LRESULT CALLBACK _WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
#endif
	
};

#endif