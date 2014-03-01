#ifndef __RENDER_WINDOW_H__
#define __RENDER_WINDOW_H__

#include "prerequisites.h"
#include "render_target.h"
#include "utils.h"

class RenderWindow : public RenderTarget
{	
public: 
	
	RenderWindow();

	virtual ~RenderWindow(); 

	virtual void create(const std::string& name, 
						unsigned int width, 
						unsigned int height, 
						bool isFullScreen, 
						const NameValueMap& params) = 0;

	virtual void destroy() = 0;


protected: 

	int m_top;
	int m_left; 
	bool m_is_fullscreen; 

};

#endif