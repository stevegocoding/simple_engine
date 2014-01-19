#ifndef __EGL_WINDOW_H__
#define __EGL_WINDOW_H__

#include "GLES2/gles2_prerequisites.h"
#include "render_window.h"

class EngineEGLContext; 
class EngineEGLWindow : public RenderWindow 
{
public: 
	
protected: 

	virtual EngineEGLContext* create_egl_context() = 0; 
	virtual void create_native_window(int& left, int& top, unsigned int w, unsigned int h, const std::string& title) = 0; 
	EGLSurface create_surface_from_window(EGLDisplay display, NativeWindowType win); 
	
	void destroy(); 

	EGLConfig* choose_config(const EGLint *attrib_list, EGLint *num_elems);
	// EGLConfig  select_config(const EGLint *min_attrib_list, const EGLint *max_attrib_list); 

	NativeWindowType m_window;
	NativeDisplayType m_native_display;
	EGLDisplay m_egl_display; 
	EGLConfig m_egl_config;
	EGLSurface m_egl_surface; 
	EngineEGLContext *m_context;
	
};


#endif