#ifndef __APP_WIN32_H__
#define __APP_WIN32_H__

#include <windows.h>
#include <EGL/egl.h>
#include "app.h"

#if (SIMPLE_ENGINE_PLATFORM == SE_PLATFORM_WIN32)

struct Win32PlatformInfo : public PlatformInfo
{
	HINSTANCE hInst; 
	HWND hWnd; 
	EGLNativeDisplayType nativeDisplay;
	 
	EGLDisplay eglDisplay; 
	EGLSurface eglWindowSurface; 
	EGLContext eglContext; 
};
typedef boost::shared_ptr<Win32PlatformInfo> win32platforminfo_ptr; 


struct Win32AppImpl : public AppImplBase 
{
	Win32AppImpl(App& app); 
	~Win32AppImpl();

	int init(const CreationSettings& settings); 
	void destroy();
	void update();
	
	int _create_native_wnd(const CreationSettings& settings);
	int _create_gles_context(const CreationSettings& settings);
	
	int wnd_left; 
	int wnd_top;
	int wnd_width;
	int wnd_height; 
	std::string wnd_title;
}; 


class Win32App : public App
{
public: 
	
	Win32App(const CreationSettings& settings); 
	~Win32App(); 

	static LRESULT CALLBACK _WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	virtual int init();
	virtual void destroy(); 

	int run(); 

	virtual AppImplPtr _create_impl(); 

	virtual bool is_enabled() const { return true; }

protected:

	int _create_event_mgr();
	void _destroy_event_mgr(); 
	
	int _create_input_mgr();
	void _destroy_input_mgr(); 
	
private: 
	bool _pump_events(); 
	void _show_window();
		
};

#endif 

#endif