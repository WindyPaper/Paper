#include "util/PrecompileHead.h"
#include "util/WindowEventUtil.h"

void WindowEventUtil::messagePump()
{
#if _WIN32
	MSG msg;
	while(PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
	{
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}
#endif
}

#ifdef _WIN32
LRESULT CALLBACK WindowEventUtil::winProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//消息处理后面加入
	switch (uMsg)
	{
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		exit(0);  //fix me
		break;
	default:
		break;
	}

	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}
#endif
