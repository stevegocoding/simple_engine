#ifndef __RENDER_SYSTEM_H__
#define __RENDER_SYSTEM_H__

#include "prerequisites.h"
#include "utils.h"

enum RS_TYPE
{
	RS_GLES2 = 0
};


struct RenderParams 
{
	
};

class RenderSystem
{
public: 

	RenderSystem(); 
	virtual ~RenderSystem(); 

	virtual void init(const NameValueMap& params) = 0;

	virtual void shutdown() = 0;
	
	virtual void begin_frame() = 0;

	virtual void clear_frame_buffer(unsigned int buffers, const Color& color, float depth, unsigned short stencil) = 0;

	virtual void end_frame() = 0; 

	virtual void swap_buffers() = 0;

	virtual RenderWindow* create_render_window(const std::string& name, unsigned int width, unsigned int height, bool isFullScreen = false) = 0; 

	virtual void render(const RenderParams& render_params) = 0;
	
	virtual void set_render_target(RenderTarget *render_target) = 0; 
}; 


class RenderSystemFactory
{
public: 
	
	virtual RenderSystem* create_render_system() = 0;  
};
typedef boost::shared_ptr<RenderSystemFactory> RSFactoryPtr;



#endif