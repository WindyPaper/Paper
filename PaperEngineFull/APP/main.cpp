#pragma once

#include <windows.h>
#include <stdio.h>
#include <vld.h>
#include "win32/Win32Framework.h"

int WINAPI WinMain(HINSTANCE hInstance,
				   HINSTANCE hPrevInstance,
				   LPSTR lpCmdLine,
				   int nCmdShow)
{
#ifdef _DEBUG
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
#endif

	Win32Framework framework;
	framework.init();

	framework.run();

#ifdef _DEBUG
	FreeConsole();
#endif // _DEBUG

	return 0;
}