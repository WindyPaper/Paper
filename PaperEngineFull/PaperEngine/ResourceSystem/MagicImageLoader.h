#ifndef _MAGIC_IMAGE_LOADER_H_
#define _MAGIC_IMAGE_LOADER_H_

#include "ResourceSystem/ResourceLoader.h"

class ENGINE_DLL MagicImageLoader : public ResourceLoader
{
public:
	virtual bool loadRes(Resource *res);
};

#endif