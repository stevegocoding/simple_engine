#ifndef __GLES2_SUPPORT_H__
#define __GLES2_SUPPORT_H__

#include "GLES2Prerequisites.h"


class RenderWindow; 

class GLES2Support
{
public:
	GLES2Support() {} 
	virtual ~GLES2Support() {}


	virtual RenderWindow* new_window(const std::string& name, 
									unsigned int width, 
									unsigned int height, 
									bool full_screen) = 0;
	

};

#endif