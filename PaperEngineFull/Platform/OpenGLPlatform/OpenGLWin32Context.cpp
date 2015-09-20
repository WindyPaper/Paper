#include "OpenGLPlatform/RenderPreCompileHead.h"
#include "OpenGLPlatform/OpenGLWin32Context.h"


OpenGLWin32Context::OpenGLWin32Context(HDC hdc, HGLRC glrc) :
	mHDC(hdc),
	mGlrc(glrc)
{
	
}

OpenGLWin32Context::~OpenGLWin32Context()
{
	releaseContext();
}

void OpenGLWin32Context::setCurrent()
{
	wglMakeCurrent(mHDC, mGlrc);
}

void OpenGLWin32Context::endCurrent()
{
	wglMakeCurrent(0, 0);
}

void OpenGLWin32Context::releaseContext()
{
	if (mGlrc != NULL)
	{
		wglDeleteContext(mGlrc);
		mGlrc = NULL;
		mHDC  = NULL;
	}
}


