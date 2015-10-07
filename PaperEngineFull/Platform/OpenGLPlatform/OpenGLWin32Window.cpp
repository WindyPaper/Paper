#include "OpenGLPlatform/RenderPreCompileHead.h"
#include "OpenGLPlatform/OpenGLWin32Window.h"
#include "util/WindowEventUtil.h"
#include "OpenGLPlatform/OpenGLWin32Support.h"
#include "OpenGLPlatform/OpenGLWin32Context.h"

OpenGLWin32Window::OpenGLWin32Window(OpenGLWin32Support *pOpenGLWin32Support) :
	mpOpenGLSupport(pOpenGLWin32Support),
	mWidth(0),
	mHeight(0)
{

}

OpenGLWin32Window::~OpenGLWin32Window()
{
	SAFE_DELETE(mpOpenGLWin32Context);
}

void OpenGLWin32Window::createWindow(const char *name, uint width, uint height, bool fullScreen)
{
	/*WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style               = CS_OWNDC;
	wcex.lpfnWndProc     = WindowEventUtil::winProc;
	wcex.cbClsExtra          = 0;
	wcex.cbWndExtra          = 0;
	wcex.hInstance          = 0;
	wcex.hIcon               = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor          = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground     = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName     = 0;
	wcex.lpszClassName     = "PaperGLWindow";*/
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof( WNDCLASSEX );
	wcex.style = CS_OWNDC;
	wcex.lpfnWndProc = WindowEventUtil::winProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = GetModuleHandle(0);
	wcex.hIcon = LoadIcon( NULL, IDI_APPLICATION );
	wcex.hCursor = LoadCursor( NULL, IDC_ARROW );
	wcex.hbrBackground = (HBRUSH) GetStockObject( WHITE_BRUSH );
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = "RenderWindow";
	wcex.hIconSm = LoadIcon( NULL, IDI_WINLOGO );
	RegisterClassEx( &wcex );

	//RegisterClassEx(&wcex);

	//mHWnd = CreateWindowEx(0, "PaperGLWindow", name, WS_VISIBLE, 0, 0, width, height, 0, 0, 0, this);
	//mHWnd = CreateWindow("PaperGLWindow", "RenderWindow", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, GetModuleHandle(NULL), NULL);

	RECT ajustRect = { 0, 0, width, height };
	AdjustWindowRect(&ajustRect, WS_OVERLAPPEDWINDOW, FALSE);

	mHWnd = CreateWindowEx(0, "RenderWindow", name, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, ajustRect.right - ajustRect.left, ajustRect.bottom - ajustRect.top, NULL, NULL, GetModuleHandle(0), NULL);
	//int createWinInt = GetLastError();

	assert(mHWnd);

	mWidth = width;
	mHeight = height;

	//HDC old_hdc = wglGetCurrentDC();
	//HGLRC old_context = wglGetCurrentContext();

	//RECT rc;
	// top and left represent outer window position
	//GetWindowRect(mHWnd, &rc);
	//mTop = rc.top;
	//mLeft = rc.left;
	// width and height represent drawable area only
	//GetClientRect(mHWnd, &rc);
	//mWidth = rc.right;
	//mHeight = rc.bottom;

	mHDC = GetDC(mHWnd);
	mpOpenGLSupport->setupPixFormat(mHDC, 32, 0, false); //测试用

	// 暂时不考虑多窗口问题
	//HDC old_hdc = wglGetCurrentDC();
	//HGLRC old_context = wglGetCurrentContext();
	mGlrc = wglCreateContext(mHDC);
	mpOpenGLWin32Context = new OpenGLWin32Context(mHDC, mGlrc);
	mpOpenGLWin32Context->setCurrent();

	ShowWindow(mHWnd, SW_SHOW);
	UpdateWindow(mHWnd);
	glewInit();
}

void OpenGLWin32Window::swapBuffer()
{
	//updateWinSize();

	::SwapBuffers(mHDC);
}

int OpenGLWin32Window::getWidth() const
{
	return mWidth;
}

int OpenGLWin32Window::getHeight() const
{
	return mHeight;
}

void OpenGLWin32Window::updateWinSize()
{
	RECT rc;
	// top and left represent outer window position
	GetWindowRect(mHWnd, &rc);
	int top = rc.top;
	int left = rc.left;
	// width and height represent drawable area only
	GetClientRect(mHWnd, &rc);

	if (mWidth == rc.right && mHeight == rc.bottom)
		return;

	mWidth = rc.right - rc.left;
	mHeight = rc.bottom - rc.top;

	//需要设置摄像机的viewport
	//need set viewport of camera
	//glViewport(0, 0, mWidth, mHeight);

	mpOpenGLWin32Context->setCurrent();
}

HWND OpenGLWin32Window::getWinHandle() const
{
	return mHWnd;
}
