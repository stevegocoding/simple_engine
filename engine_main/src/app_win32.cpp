#include "app_win32.h"
#include "render_system.h"
#include "factory_manager.h"
#include "input_manager_win32.h"
#include <boost/make_shared.hpp>

#if (SIMPLE_ENGINE_PLATFORM == SE_PLATFORM_WIN32)

GlobalsBasePtr g_globals;

Win32AppImpl::Win32AppImpl(App& app)
	: AppImplBase(app)
{
	
}

Win32AppImpl::~Win32AppImpl()
{
	
}

int Win32AppImpl::init(const CreationSettings& settings)
{	
	/** Create win32 window */
	int ret = 0;

	ret = _create_native_wnd(settings); 
	assert (ret == 0);  

	ret = _create_gles_context(settings); 
	assert (ret == 0); 
	
	return 0; 
}

void Win32AppImpl::destroy()
{
	win32platforminfo_ptr win32_platforminfo = 
		boost::shared_static_cast<Win32PlatformInfo>(m_app.get_platform_info());
	
	ReleaseDC((HWND)win32_platforminfo->hWnd,(HDC)win32_platforminfo->nativeDisplay); 
	DestroyWindow((HWND)win32_platforminfo->hWnd); 
}

void Win32AppImpl::update()
{
	
} 

int Win32AppImpl::_create_native_wnd(const CreationSettings& settings)
{
	int left = settings.wnd_pos_x;
	int top = settings.wnd_pos_y; 
	unsigned int w = settings.wnd_width;
	unsigned int h = settings.wnd_height; 
	const std::string& title = settings.wnd_title; 

	// The global instance
	HINSTANCE hInstance  = GetModuleHandle( NULL );

	// Register the window class
	WNDCLASS wc;
	wc.style          = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;  // Window style
	wc.lpfnWndProc    = Win32App::_WndProc;         // WndProc message handler
	wc.cbClsExtra     = 0;                                   // No extra window data
	wc.cbWndExtra     = 0;                                   // No extra window data
	wc.hInstance      = hInstance;                           // Instance
	//wc.hIcon          = (HICON)LoadImage( NULL, "Adreno.ico", IMAGE_ICON, 16, 16, LR_LOADFROMFILE );
	wc.hIcon			= NULL; 
	wc.hCursor        = LoadCursor( NULL, IDC_ARROW );       // Cursor
	wc.hbrBackground  = NULL;                                // No Background
	wc.lpszMenuName   = NULL;                                // No menu
	wc.lpszClassName  = title.c_str();                 // Set the class name

	if( FALSE == RegisterClass(&wc) )
	{
		assert(false);
		return 1;
	}

	// Adjust the window size to fit our rectangle
	DWORD dwWindowStyle = WS_SYSMENU | WS_MINIMIZEBOX | WS_CAPTION | WS_BORDER;
	
	RECT rcWindow; 
	SetRect( &rcWindow, left, top, w, h);
	AdjustWindowRect( &rcWindow, dwWindowStyle, FALSE );
	
	// Create the parent window
	HWND hParentWnd = CreateWindowEx(
		WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,    // Extended style
		title.c_str(),							// Class
		title.c_str(),							// Title
		dwWindowStyle,                         // Style
		50 + rcWindow.left, 50 + rcWindow.top, // Position
		(rcWindow.right-rcWindow.left),        // Width
		(rcWindow.bottom-rcWindow.top),        // Height
		NULL,                                  // No parent window
		NULL,                                  // No menu
		hInstance,                             // Instance
		NULL );                                // Creation parameter
	if( NULL == hParentWnd )
	{
		assert(false);
		return 1;
	}
	
	// Pass application data pointer to the windows for later use
	SetWindowLong( hParentWnd, GWLP_USERDATA, (LONG)&(this->m_app));

	// Note: We delay showing the window until after Initialization() succeeds
	// Otherwise, an unsightly, empty window briefly appears during initialization
	win32platforminfo_ptr win32_platforminfo = boost::shared_static_cast<Win32PlatformInfo>(m_app.get_platform_info());
	
	win32_platforminfo->hWnd = hParentWnd;
	win32_platforminfo->hInst = hInstance; 
	win32_platforminfo->nativeDisplay = GetDC(hParentWnd);
	
	return 0; 
}

int Win32AppImpl::_create_gles_context(const CreationSettings& settings)
{
	win32platforminfo_ptr win32_platforminfo = boost::shared_static_cast<Win32PlatformInfo>(m_app.get_platform_info());
	
	/** Get EGL display */
	EGLDisplay egl_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

	if (egl_display == EGL_NO_DISPLAY)
	{
		assert(false && "eglGetDisplay FAILED!"); 
		return 1; 
	}

	/** Initialize EGL */
	EGLint major, minor; 
	if (!eglInitialize(egl_display, &major, &minor))
	{
		assert(false && "eglInitialize FAILED!"); 
		return 1; 
	}

	/** Choose Config */
	EGLConfig config; 
	EGLint num_configs; 
	
	const EGLint config_atrbs[] = {
		EGL_SURFACE_TYPE,			EGL_WINDOW_BIT,
		EGL_NATIVE_RENDERABLE,		EGL_FALSE, 
		EGL_RENDERABLE_TYPE,		EGL_OPENGL_ES2_BIT, 
		EGL_DEPTH_SIZE,				settings.depthbuf_size, 
		EGL_RED_SIZE,				5, 
		EGL_GREEN_SIZE,				6, 
		EGL_BLUE_SIZE,				5,
		EGL_ALPHA_SIZE,				0, 
		EGL_NONE
	};
	
	if (!eglChooseConfig(egl_display, config_atrbs, &config, 1, &num_configs))
	{
		assert(false && "eglChooseConfig FAILED!");
		eglTerminate(egl_display);
		return 1; 
	} 

	/** Create Window Surface */
	EGLSurface egl_wnd_surface;
	const EGLint surface_atrbs[] = {EGL_NONE}; 
	egl_wnd_surface = eglCreateWindowSurface(egl_display, 
										config, 
										(EGLNativeWindowType)win32_platforminfo->hWnd,
										surface_atrbs); 
	if (egl_wnd_surface == EGL_NO_SURFACE)
	{
		assert(false && "eglCreateWindowSurface FAILED"); 
		return 1; 
	}

	/** Create EGL Context */
	EGLContext egl_context; 
	const EGLint context_atrbs[] = {
		EGL_CONTEXT_CLIENT_VERSION, 2, 
		EGL_NONE
	}; 
	egl_context = eglCreateContext(egl_display, config, EGL_NO_CONTEXT, context_atrbs); 
	if (egl_context == EGL_NO_CONTEXT)
	{
		assert(false && "eglCreateContext FAILED!"); 
		return 1; 
	}

	eglBindAPI(EGL_OPENGL_ES_API);

	if (!eglMakeCurrent(egl_display,
						egl_wnd_surface, 
						egl_wnd_surface,
						egl_context))
	{
		assert(false && "eglMakeCurrent FAILED!"); 
		return 1; 
	}
	
	eglSwapInterval(egl_display, settings.wait_vsync? 1 : 0);

	win32_platforminfo->eglDisplay = egl_display; 
	win32_platforminfo->eglWindowSurface = egl_wnd_surface;  
	win32_platforminfo->eglContext = egl_context; 

	return 0;
} 

//////////////////////////////////////////////////////////////////////////

Win32App::Win32App(const CreationSettings& settings)
	: App(settings)
{
	m_pimp = _create_impl(); 
	m_platform_info = boost::make_shared<Win32PlatformInfo>();
}

Win32App::~Win32App()
{
	
}

AppImplPtr Win32App::_create_impl() 
{
	AppImplPtr pimp = AppImplPtr(new Win32AppImpl(*this)); 
	return pimp;  
}

LRESULT Win32App::_WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{ 
	switch(uMsg)
	{
	case WM_DESTROY:
	case WM_CLOSE: 
		{		
			PostQuitMessage(0);
			return 0;			
		}
		break;

	case WM_MOUSEMOVE: 
	case WM_LBUTTONDOWN: 
	case WM_LBUTTONUP:
		{
			get_app()->get_input_mgr()->
				on_wnd_events(Win32NativeWndMsg(uMsg, lParam, wParam)); 
		}	
		break; 
	}
	
	
	
	
	return DefWindowProc(hWnd, uMsg, wParam, lParam); 
}

bool Win32App::_pump_events()
{ 
	MSG msg; 
	
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		if( msg.message == WM_QUIT )
			return false;

		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}
	
	return true; 
}

int Win32App::run()
{
	/** Show App's Window */ 
	_show_window(); 

	while (_pump_events())
	{
		update(); 
		
		render_frame();

		present(); 
	}

	return 0; 
}

void Win32App::_show_window()
{
	win32platforminfo_ptr win32_platforminfo = 
		boost::shared_static_cast<Win32PlatformInfo>(get_platform_info());
	
	ShowWindow(win32_platforminfo->hWnd, TRUE);
	SetForegroundWindow(win32_platforminfo->hWnd); 
	SetFocus(win32_platforminfo->hWnd); 
}

//////////////////////////////////////////////////////////////////////////

void init_globals()
{
	if (g_globals == GlobalsBasePtr())
	{
		g_globals.reset(new GlobalsBase());
	} 
}

GlobalsBasePtr get_globals()
{
	return g_globals; 
}

void destroy_globals()
{
	g_globals.reset(); 
}

int RunApp()
{
	App *win32_app = CreateApp(); 

	int result = 0;
	if (win32_app && win32_app->init() == 0)
	{
		result = win32_app->run();
		win32_app->destory(); 
	}
	
	return result; 
}

/** Test App */
App* CreateApp()
{
	CreationSettings settings;
	settings.wnd_pos_x = 100; 
	settings.wnd_pos_y = 100; 
	settings.wnd_width = 1024; 
	settings.wnd_height = 768; 

	Win32App *app = new Win32App(settings); 
	
	return app; 
}

App* get_app() 
{
	assert(g_globals);
	return g_globals->app; 
}


int CALLBACK WinMain(
	HINSTANCE   hInstance,
	HINSTANCE   hPrevInstance,
	LPSTR       lpCmdLine,
	int         nCmdShow) 
{
	int result = RunApp(); 
	
	return result; 
} 

#endif 