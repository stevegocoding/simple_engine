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
	{}
};

class AndroidNativeApp : public App
{
public: 
	
	AndroidNativeApp(const CreationSettings& settings);
	~AndroidNativeApp();
	
	virtual int init();
	virtual void destroy(); 
	virtual void update(); 
	
private:
	platforminfo_ptr _create_platform_info(); 
	int _create_gles_context(const CreationSettings& settings);
	EGLConfig _choose_egl_cfg(EGLDisplay display, EGLConfig *configs, int num_configs);
};


#endif
