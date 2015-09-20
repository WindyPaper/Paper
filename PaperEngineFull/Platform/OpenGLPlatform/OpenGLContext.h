#ifndef _OPENGL_CONTEXT_H_
#define _OPENGL_CONTEXT_H_

#include "util/Platform.h"

class ENGINE_DLL OpenGLContext
{
public:
	virtual void setCurrent() = 0;
	virtual void endCurrent() = 0;

	virtual void releaseContext() {}
};

#endif