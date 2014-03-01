#ifndef __GLES2_RENDER_SYSTEM_H__
#define __GLES2_RENDER_SYSTEM_H__

#include "render_system.h"


class GLES2Context;
class GLES2RenderSystem : public RenderSystem 
{
public: 
	
	GLES2RenderSystem(); 
	virtual ~GLES2RenderSystem(); 

	void init(const NameValueMap& params); 
	void shutdown();
	void begin_frame(); 
	void end_frame(); 

	void clear_frame_buffer(unsigned int buffers, const Color& color, float depth, unsigned short stencil);

	RenderWindow* create_render_window(const std::string& name, 
									unsigned int width, 
									unsigned int height, 
									bool isFullScreen = false);
	
	void render(const RenderParams& render_params);

	void set_render_target(RenderTarget *render_target); 

	void swap_buffers(); 

private:

	GLES2Context *m_main_context; 
	
};


class GLES2RenderSystemFactory : public RenderSystemFactory
{
public: 
	RenderSystem* create_render_system(); 
};




#endif