#ifndef _OPENGL_WIN32_SUPPORT_H_
#define _OPENGL_WIN32_SUPPORT_H_

#include "OpenGLPlatform/OpenGLSupport.h"
#include <windows.h>

class OpenGLWin32Window;

class ENGINE_DLL OpenGLWin32Support : public OpenGLSupport
{
public:
	OpenGLWin32Support();
	virtual ~OpenGLWin32Support();

	virtual void createWindow(const char *windowtitle);
	int setupPixFormat(HDC hdc, int colourDepth, int multisample, bool hwGamma);
	void swapBuffer();

	const OpenGLWin32Window *getMainRenderWindow() const;

private:
	OpenGLWin32Window *mpMainWindow;
};

#endif