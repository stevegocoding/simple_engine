#include "GLES2/EGL/engine_egl_context.h"

EngineEGLContext::EngineEGLContext(EGLDisplay egl_display, EGLConfig egl_config, EGLSurface egl_drawable)
	: m_egl_display(egl_display)
	, m_egl_config(egl_config)
	, m_egl_drawable(egl_drawable)
	, m_elg_context(0)
{
	
	
}

EngineEGLContext::~EngineEGLContext()
{
	
}

void EngineEGLContext::set_current()
{
	EGLBoolean ret = eglMakeCurrent(m_egl_display, m_egl_drawable, m_egl_drawable, m_elg_context); 
	assert(ret); 
}

void EngineEGLContext::end_current()
{
	eglMakeCurrent(m_egl_display, 0, 0, 0); 
}

void EngineEGLContext::release_context()
{
}