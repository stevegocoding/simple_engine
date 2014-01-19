#ifndef __ENGINE_EGL_WIN32_WINDOW_H__
#define __ENGINE_EGL_WIN32_WINDOW_H__

#include "GLES2/EGL/engine_egl_window.h" 
#include "utils.h"

class EngineEGLWin32Window : public EngineEGLWindow 
{
public: 

	EngineEGLWin32Window();
	~EngineEGLWin32Window(); 

	void create(const std::string& name, 
				unsigned int width, 
				unsigned int height, 
				bool isFullScreen, 
				const NameValueMap& params); 

private:
	EngineEGLContext* create_egl_context(); 
	void create_native_window(int& left, int& top, unsigned int w, unsigned int h, const std::string& title); 
	void create_native_window_sdl(int& left, int& top, unsigned int w, unsigned int h, const std::string& title);

};


RenderWindow* create_new_window(const std::string& name, 
								unsigned int width, 
								unsigned int height, 
								bool fullscreen, 
								const NameValueMap& params); 



#endif