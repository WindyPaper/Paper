#ifndef _OPENGL_WIN32_WINDOW_H_
#define _OPENGL_WIN32_WINDOW_H_

#include <windows.h>
#include "GL/glew.h"
#include "util/Platform.h"
#include "util/Engine_Define.h"

class OpenGLWin32Support;
class OpenGLWin32Context;

class ENGINE_DLL OpenGLWin32Window
{
public:
	OpenGLWin32Window(OpenGLWin32Support *pOpenGLWin32Support);
	virtual ~OpenGLWin32Window();

	void createWindow(const char *name, uint width, uint height, bool fullScreen);
	void swapBuffer();

	int getWidth() const;
	int getHeight() const;
	
	HWND getWinHandle() const;

private:
	OpenGLWin32Support *mpOpenGLSupport;
	HWND mHWnd;
	HDC mHDC;
	HGLRC mGlrc;
	OpenGLWin32Context *mpOpenGLWin32Context;

	int mWidth;
	int mHeight;
};

#endif