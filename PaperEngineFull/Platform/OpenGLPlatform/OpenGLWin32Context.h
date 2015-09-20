#ifndef _OPENGL_WIN32_CONTEXT_H_
#define _OPENGL_WIN32_CONTEXT_H_

#include "OpenGLPlatform/OpenGLSupport.h"
#include "OpenGLPlatform/OpenGLContext.h"
#include <GL/glew.h>
#include <windows.h>

class ENGINE_DLL OpenGLWin32Context : public OpenGLContext
{
public:
	OpenGLWin32Context(HDC hdc, HGLRC glrc);
	virtual ~OpenGLWin32Context();

	virtual void setCurrent();
	virtual void endCurrent();

	virtual void releaseContext();

private:
	HDC mHDC;
	HGLRC mGlrc;
};

#endif