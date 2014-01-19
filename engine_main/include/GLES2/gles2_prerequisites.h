#ifndef __GLES2_PREREQUISITES_H__
#define __GLES2_PREREQUISITES_H__

#include "prerequisites.h"
#include "platform.h"

#if (SIMPLE_ENGINE_PLATFORM == SE_PLATFORM_WIN32)

	#include "GLES2/gl2platform.h"
	#include "GLES2/gl2.h"
	#include "GLES2/gl2ext.h "
	#include "EGL/egl.h"

#endif


#if ENABLE_GL_CHECK

#define EGL_CHECK_ERROR \ 
	{ \
		int e = eglGetError(); \ 
	}

#else

#define EGL_CHECK_ERROR {} 

#endif 





#endif