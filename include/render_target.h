#ifndef __RENDER_TARGET_H__
#define __RENDER_TARGET_H__


#include "prerequisites.h"

class RenderTarget 
{
	
public:
	RenderTarget(); 
	~RenderTarget(); 
	
	virtual void swap_buffers(bool vysnc = true)
	{ (void)vysnc; }

	
protected:
	
	unsigned int m_width;
	unsigned int m_height; 
	
};


#endif