#ifndef __RENDER_WINDOW_H__
#define __RENDER_WINDOW_H__

#include "prerequisites.h"
#include "render_target.h"

class RenderWindow : public RenderTarget
{	
public: 
	
	RenderWindow();

	virtual ~RenderWindow(); 

	virtual void create(const std::string& name, unsigned int width, unsigned int height, bool isFullScreen = false) = 0;

	virtual void destroy() = 0;


protected: 

	int m_pos_x, m_pos_y; 
	

};

#endif