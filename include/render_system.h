#ifndef __RENDER_SYSTEM_H__
#define __RENDER_SYSTEM_H__

#include "prerequisites.h"

struct RenderParams 
{
	
};

class RenderSystem
{
public: 

	RenderSystem(); 
	virtual ~RenderSystem(); 

	virtual void init() = 0;

	virtual void shutdown() = 0;
	
	virtual void begin_frame() = 0;

	virtual void clear_frame_buffer(unsigned int buffers, const Color& color, float depth, unsigned short stencil) = 0;

	virtual void end_frame() = 0; 

	virtual void swap_buffers() = 0;

	virtual RenderWindow* create_render_window(const std::string& name, unsigned int width, unsigned int height, bool isFullScreen = false) = 0; 

	virtual void render(const RenderParams& render_params) = 0;
	
	virtual void set_render_target(RenderTarget *render_target) = 0; 
	
};


#endif