#include "OpenGLPlatform/RenderPreCompileHead.h"
#include "util/Global.h"
#include "OpenGLPlatform/OpenGLWin32Support.h"
#include "OpenGLPlatform/OpenGLWin32Window.h"

OpenGLWin32Support::OpenGLWin32Support() :
	mpMainWindow(0)
{

}

OpenGLWin32Support::~OpenGLWin32Support()
{
	SAFE_DELETE(mpMainWindow);
}

void OpenGLWin32Support::createWindow(const char *windowtitle)
{
	mpMainWindow = new OpenGLWin32Window(this);
	mpMainWindow->createWindow(windowtitle, WIN_WIDTH, WIN_HEIGHT, false);
}

int OpenGLWin32Support::setupPixFormat(HDC hdc, int colourDepth, int multisample, bool hwGamma)
{
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(pfd));
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = (colourDepth > 16)? 24 : colourDepth;
	pfd.cAlphaBits = (colourDepth > 16)? 8 : 0;
	pfd.cDepthBits = 32;
	pfd.cStencilBits = 8;

	int format = 0;

	//int useHwGamma = hwGamma? GL_TRUE : GL_FALSE;

	//if (multisample && (!mHasMultisample || !mHasPixelFormatARB))
	//	return false;

	//if (hwGamma && !mHasHardwareGamma)
	//	return false;

	/*if ((multisample || hwGamma) && WGLEW_GET_FUN(__wglewChoosePixelFormatARB))
	{

		// Use WGL to test extended caps (multisample, sRGB)
		std::vector<int> attribList;
		attribList.push_back(WGL_DRAW_TO_WINDOW_ARB); attribList.push_back(GL_TRUE);
		attribList.push_back(WGL_SUPPORT_OPENGL_ARB); attribList.push_back(GL_TRUE);
		attribList.push_back(WGL_DOUBLE_BUFFER_ARB); attribList.push_back(GL_TRUE);
		attribList.push_back(WGL_SAMPLE_BUFFERS_ARB); attribList.push_back(GL_TRUE);
		attribList.push_back(WGL_ACCELERATION_ARB); attribList.push_back(WGL_FULL_ACCELERATION_ARB);
		attribList.push_back(WGL_COLOR_BITS_ARB); attribList.push_back(pfd.cColorBits);
		attribList.push_back(WGL_ALPHA_BITS_ARB); attribList.push_back(pfd.cAlphaBits);
		attribList.push_back(WGL_DEPTH_BITS_ARB); attribList.push_back(24);
		attribList.push_back(WGL_STENCIL_BITS_ARB); attribList.push_back(8);
		attribList.push_back(WGL_SAMPLES_ARB); attribList.push_back(multisample);
		if (useHwGamma && checkExtension("WGL_EXT_framebuffer_sRGB"))
		{
			attribList.push_back(WGL_FRAMEBUFFER_SRGB_CAPABLE_EXT); attribList.push_back(GL_TRUE);
		}
		// terminator
		attribList.push_back(0);


		UINT nformats;
		// ChoosePixelFormatARB proc address was obtained when setting up a dummy GL context in initialiseWGL()
		// since glew hasn't been initialized yet, we have to cheat and use the previously obtained address
		if (!WGLEW_GET_FUN(__wglewChoosePixelFormatARB)(hdc, &(attribList[0]), NULL, 1, &format, &nformats) || nformats <= 0)
			return false;
	}
	else
	{
		format = ChoosePixelFormat(hdc, &pfd);
	}*/

	format = ChoosePixelFormat(hdc, &pfd);
	return (format && SetPixelFormat(hdc, format, &pfd));
}

void OpenGLWin32Support::swapBuffer()
{
	mpMainWindow->swapBuffer();
}

const OpenGLWin32Window * OpenGLWin32Support::getMainRenderWindow() const
{
	return mpMainWindow;
}
