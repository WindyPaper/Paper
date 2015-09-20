#ifndef _WINDOW_EVENT_UTIL_H_
#define _WINDOW_EVENT_UTIL_H_

#include "util/Platform.h"

#ifdef _WIN32
#include <windows.h>
#endif

class ENGINE_DLL WindowEventUtil
{
public:
	static void messagePump();

#ifdef _WIN32
	static LRESULT CALLBACK winProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
#endif
};

#endif