#ifndef _RESOURCE_LOADER_H_
#define _RESOURCE_LOADER_H_

#include "util/Platform.h"

class Resource;

class ENGINE_DLL ResourceLoader
{
public:
	virtual bool loadRes(Resource *res) = 0;
};

#endif