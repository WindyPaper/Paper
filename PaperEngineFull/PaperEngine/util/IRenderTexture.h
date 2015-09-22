#ifndef _I_RENDER_TEXTURE_H_
#define _I_RENDER_TEXTURE_H_

#include "util/Engine_Define.h"

class IRenderTexture
{
public:
	virtual ~IRenderTexture() {}

	virtual bool init(const uint width, const uint height) = 0;
	virtual void bindForWriting() = 0;
	virtual void bindForReading(const uint texUnit) = 0;
};

#endif