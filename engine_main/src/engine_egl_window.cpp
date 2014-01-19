#include "GLES2/EGL/engine_egl_window.h"


void EngineEGLWindow::destroy()
{
	
}

EGLSurface EngineEGLWindow::create_surface_from_window(EGLDisplay display, NativeWindowType win)
{ 
	EGLSurface surface;
	
	surface = eglCreateWindowSurface(display, m_egl_config, (EGLNativeWindowType)win, NULL);  

	return surface; 
}

/*
EGLConfig  EngineEGLWindow::select_config(const EGLint *min_attrib_list, const EGLint *max_attrib_list)
{
	
}
*/

EGLConfig* EngineEGLWindow::choose_config(const EGLint *attrib_list, EGLint *num_elems)
{
	EGLConfig *configs; 

	if (eglChooseConfig(m_egl_display, attrib_list, NULL, 0, num_elems) == EGL_FALSE)
	{
		 *num_elems = 0; 
		 return NULL; 
	}
	
	EGL_CHECK_ERROR

	configs = (EGLConfig*) malloc(*num_elems * sizeof(EGLConfig));
	
	if (eglChooseConfig(m_egl_display, attrib_list, configs, *num_elems, num_elems) == EGL_FALSE)
	{
		*num_elems = 0; 
		free(configs);
		return NULL; 
	}

	EGL_CHECK_ERROR

	return configs; 
}