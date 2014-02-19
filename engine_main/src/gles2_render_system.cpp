#include "gles2_render_system.h"

GLES2RenderSystem::GLES2RenderSystem()
{
	
}

GLES2RenderSystem::~GLES2RenderSystem()
{
	
}

void GLES2RenderSystem::init(const NameValueMap& params)
{
}

void GLES2RenderSystem::shutdown()
{
	
}

void GLES2RenderSystem::begin_frame()
{
	
}

void GLES2RenderSystem::end_frame()
{
	
}

void GLES2RenderSystem::clear_frame_buffer(unsigned int buffers, const Color& color, float depth, unsigned short stencil)
{
	
}

void GLES2RenderSystem::swap_buffers()
{
	
}

RenderWindow* GLES2RenderSystem::create_render_window(const std::string& name, unsigned int width, unsigned int height, bool isFullScreen)
{
	return NULL; 
}

void GLES2RenderSystem::render(const RenderParams& render_params)
{
	
}

void GLES2RenderSystem::set_render_target(RenderTarget *render_target)
{
	
}

////////////////////////////////////////////////////////////////////////// 

RenderSystem* GLES2RenderSystemFactory::create_render_system()
{
	return new GLES2RenderSystem(); 
}