#ifndef _PLATFORM_H_
#define _PLATFORM_H_

// 引擎项目宏定义

#if defined(_WIN32)
#	if defined(BUILD_STATIC)
#		define ENGINE_DLL
#	elif defined(ENGINE_DLL_EXPORT)
#		define ENGINE_DLL __declspec(dllexport)
#	else
#		define ENGINE_DLL __declspec(dllimport)
#	endif
#endif

#include <vector>
#include <string>
#include <map>
#include <list>

#if defined(_WIN32)
#include <windows.h>
#endif

#include <stdio.h>
#include <memory.h>
#include <iostream>

#include <assert.h>
#include <fstream>
#include <sstream>

#include "util/sigslot.h"

#include "XNAMath/xnamath.h"

#endif