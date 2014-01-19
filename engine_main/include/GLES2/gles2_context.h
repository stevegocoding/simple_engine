#ifndef __GLES_CONTEXT_H__
#define __GLES_CONTEXT_H__

#include "gles2_prerequisites.h"


class GLES2Context 
{
public:
	GLES2Context(); 
	virtual ~GLES2Context(); 

	/**
	 *	Enable the context. All subsequent rendering commands will go here.
	 */
	virtual void set_current() = 0; 
	
	/**
	 *	This is called before another context is made current. By default,
	 *	nothing is done here.
	 */
	virtual void end_current() = 0;

	/**
	 *	Release the context
	 */
	virtual void release_context() = 0;

	bool get_initialized() const { return m_initialized; }
	void set_initialized() { m_initialized = true; }
	
protected:
	
	bool m_initialized;
	
	
	
};



#endif