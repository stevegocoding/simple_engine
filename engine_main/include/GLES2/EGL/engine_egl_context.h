#ifndef __ENGINE_EGL_CONTEXT_H__
#define __ENGINE_EGL_CONTEXT_H__

#include "GLES2/gles2_context.h"

class EngineEGLContext : public GLES2Context
{
public:
	
	EngineEGLContext(EGLDisplay egl_display, EGLConfig egl_config, EGLSurface egl_drawable);
	virtual ~EngineEGLContext(); 
	
	virtual void set_current();
	virtual void end_current();

	virtual void release_context(); 
	
protected:
	
	EGLDisplay m_egl_display; 
	EGLConfig m_egl_config;
	EGLSurface m_egl_drawable; 
	EGLContext m_elg_context;  

};


#endif