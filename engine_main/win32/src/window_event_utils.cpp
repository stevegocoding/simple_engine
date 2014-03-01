#include "window_event_utils.h"

#if SIMPLE_ENGINE_PLATFORM == SE_PLATFORM_WIN32 
LRESULT CALLBACK WindowEventUtils::_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{
	return DefWindowProc( hWnd, uMsg, wParam, lParam ); 
}
#endif 


void WindowEventUtils::dispatch_message()
{
#if SIMPLE_ENGINE_PLATFORM == SE_PLATFORM_WIN32 

	MSG msg; 

	while( PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE) )
	{
		TranslateMessage(&msg); 
		DispatchMessage(&msg);
	}
	
#endif
}