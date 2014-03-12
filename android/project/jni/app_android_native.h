#ifndef __APP_ANDROID_NATIVE_H__
#define __APP_ANDROID_NATIVE_H__

#include <EGL/egl.h>
#include "app.h"


struct AndroidPlatformInfo : public PlatformInfo
{
	EGLDisplay eglDisplay; 
	EGLSurface eglWindowSurface; 
	EGLContext eglContext; 
	struct android_app *and_app;
	
	AndroidPlatformInfo()
		: PlatformInfo()
		, and_app(NULL)
		, eglContext(EGL_NO_CONTEXT)
		, eglDisplay(EGL_NO_DISPLAY)
		, eglWindowSurface(EGL_NO_SURFACE)
	{}
};
typedef boost::shared_ptr<AndroidPlatformInfo> AndroidPlatformInfoPtr; 

struct AndroidGlobals : public GlobalsBase
{
	AndroidGlobals()
		: GlobalsBase()
		, and_app(NULL)
		, is_app_initialized(false)
		, has_focus(false)
	{}
	
	struct android_app *and_app; 
	bool is_app_initialized;
	bool has_focus; 
};
typedef boost::shared_ptr<AndroidGlobals> AndroidGlobalsPtr;

class AndroidNativeApp : public App
{
public: 
	
	AndroidNativeApp(const CreationSettings& settings);
	~AndroidNativeApp();
	
	virtual int init();
	virtual void destroy(); 
	virtual void update(); 
	virtual int run(); 
	virtual void present();
	virtual bool on_event(const CoreEvent& e); 
	
	int create_context();
	void destroy_context();
	
private:
	platforminfo_ptr _create_platform_info(); 
	void _create_event_mgr(); 
	void _create_input_mgr(); 
	
	void _destroy_event_mgr(); 
	void _destroy_input_mgr();
	
	int _create_gles_context(const CreationSettings& settings);
	EGLConfig _choose_egl_cfg(EGLDisplay display, EGLConfig *configs, int num_configs); 
};


#endif
