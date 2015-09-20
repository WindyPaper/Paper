#ifndef _IMAGE_LOADER_H_
#define _IMAGE_LOADER_H_

#include "ResourceSystem/ResourceLoader.h"

class ENGINE_DLL ImageLoader : public ResourceLoader
{
public:
	virtual bool loadRes(Resource *res) = 0;
};

#endif