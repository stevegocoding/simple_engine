#include "GLES2/EGL/engine_eglwin32_window.h"
#include "gles2/EGL/engine_egl_context.h"
#include "window_event_utils.h"
#include <windows.h>

const std::string WND_CLASS_NAME = "SimpleEngineGLESWindow"; 

EngineEGLWin32Window::EngineEGLWin32Window()
{
	
}

EngineEGLWin32Window::~EngineEGLWin32Window()
{
	
} 

void EngineEGLWin32Window::create_native_window(int& left, int& top, unsigned int w, unsigned int h, const std::string& title)
{ 
	HINSTANCE	hInst = GetModuleHandle(NULL);
	HWND		parent = 0;
	HMONITOR 	hMonitor = NULL;
	int			monitorIndex = -1;
	
	DWORD			dwStyle = WS_VISIBLE | WS_CLIPCHILDREN;
	DWORD			dwStyleEx = 0;
	MONITORINFOEX	monitorInfoEx;
	RECT			rc; 

	if (hMonitor == NULL) 
	{
		POINT windowAnchorPoint;
		
		// Fill in anchor point.
		windowAnchorPoint.x = left; 
		windowAnchorPoint.y = top; 

		// Get the nearest monitor to this window.
		hMonitor = MonitorFromPoint(windowAnchorPoint, MONITOR_DEFAULTTONEAREST); 
	}

	// Get the target monitor info		
	memset(&monitorInfoEx, 0, sizeof(MONITORINFOEX));
	monitorInfoEx.cbSize = sizeof(MONITORINFOEX);
	GetMonitorInfo(hMonitor, &monitorInfoEx);
	
	// No specified top left -> Center the window in the middle of the monitor
	if (left == -1 || top == -1)
	{				
		int screenw = monitorInfoEx.rcMonitor.right  - monitorInfoEx.rcMonitor.left;
		int screenh = monitorInfoEx.rcMonitor.bottom - monitorInfoEx.rcMonitor.top;

		SetRect(&rc, 0, 0, w, h);
		AdjustWindowRect(&rc, dwStyle, false);

		// clamp window dimensions to screen size
		int outerw = (rc.right-rc.left < screenw)? rc.right-rc.left : screenw;
		int outerh = (rc.bottom-rc.top < screenh)? rc.bottom-rc.top : screenh;

		if (left == -1)
			left = monitorInfoEx.rcMonitor.left + (screenw - outerw) / 2;
		else if (monitorIndex != -1)
			left += monitorInfoEx.rcMonitor.left;

		if (top == -1)
			top = monitorInfoEx.rcMonitor.top + (screenh - outerh) / 2;
		else if (monitorIndex != -1)
			top += monitorInfoEx.rcMonitor.top;
	}
	else if (monitorIndex != -1)
	{
		left += monitorInfoEx.rcMonitor.left;
		top += monitorInfoEx.rcMonitor.top;
	}

	m_width = w;
	m_height = h;
	m_top = top;
	m_left = left;
	
	if (m_is_fullscreen)
	{

	}
	else 
	{
		
		int screenw = GetSystemMetrics(SM_CXSCREEN); 
		int screenh = GetSystemMetrics(SM_CYSCREEN); 		
	}

	// register class and create window
	WNDCLASS wc = { CS_OWNDC, WindowEventUtils::_WndProc, 0, 0, hInst,
		LoadIcon(NULL, IDI_APPLICATION), LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)GetStockObject(BLACK_BRUSH), NULL, WND_CLASS_NAME.c_str() };
	RegisterClass(&wc);

	if (m_is_fullscreen)
	{
		DEVMODE display_device_mode; 
		
		memset(&display_device_mode, 0, sizeof(display_device_mode)); 
		display_device_mode.dmSize = sizeof(DEVMODE);
		display_device_mode.dmBitsPerPel = m_bits_per_pixel;
		display_device_mode.dmPelsWidth = m_width;
		display_device_mode.dmPelsWidth = m_height; 
		display_device_mode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT; 		
	}

	// Pass pointer to self as WM_CREATE parameter
	m_window = CreateWindowEx(dwStyleEx, WND_CLASS_NAME.c_str(), title.c_str(),
		dwStyle, m_left, m_top, m_width, m_height, parent, 0, hInst, this);

	// top and left represent outer window position 
	GetWindowRect(m_window, &rc);

	m_top = rc.top;
	m_left = rc.left; 

	// width and height represent drawable area only
	GetClientRect(m_window, &rc); 
	m_width = rc.right;
	m_height = rc.bottom; 

	m_native_display = GetDC(m_window);
	m_egl_display = eglGetDisplay(m_native_display); 
	
	if (m_egl_display == EGL_NO_DISPLAY)
	{
		m_egl_display = eglGetDisplay(EGL_DEFAULT_DISPLAY); 
	}

	eglInitialize(m_egl_display, NULL, NULL); 
	m_egl_surface = create_surface_from_window(m_egl_display, m_window); 

}

void EngineEGLWin32Window::create_native_window_sdl(int& left, int& top, unsigned int w, unsigned int h, const std::string& title)
{
		
}

EngineEGLContext* EngineEGLWin32Window::create_egl_context()
{ 
	EngineEGLContext* ctx = new EngineEGLContext(m_egl_display, m_egl_config, m_egl_surface); 
	return ctx; 
}

void EngineEGLWin32Window::create(const std::string& name, 
								  unsigned int width, 
								  unsigned int height, 
								  bool isFullScreen, 
								  const NameValueMap& params)
{
	std::string title = name;
	int left = 0; 
	int top = 0; 
	
	NameValueMap::const_iterator cit;
	if ( (cit = params.find("left")) != params.end())
	{
		left = boost::any_cast<int>(cit->second);
	} 
	if ( (cit = params.find("top")) != params.end() )
	{
		top = boost::any_cast<int>(cit->second); 
	}
	
	if (!m_egl_config)
	{
		const int config_attribs[] = 
		{
			EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
			EGL_RED_SIZE, 8,
			EGL_GREEN_SIZE, 8, 
			EGL_BLUE_SIZE, 8, 
			EGL_DEPTH_SIZE, 24,  
			EGL_NONE
		}; 

		EGLint num_configs; 
		eglChooseConfig(m_egl_display, config_attribs, &m_egl_config, 1, &num_configs);  
	}
	
	create_native_window(left, top, width, height, title);  
	
	m_context = create_egl_context();

	m_width = width;
	m_height = height; 
	m_left = left; 
	m_top = top; 
}

////////////////////////////////////////////////////////////////////////// 

RenderWindow* create_new_window(const std::string& name, 
								unsigned int width, 
								unsigned int height, 
								bool fullscreen, 
								const NameValueMap& params)
{
	EngineEGLWin32Window *window = new EngineEGLWin32Window();
	window->create(name, width, height, fullscreen, params); 

	return window; 
}