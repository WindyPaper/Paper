#ifndef _MAGICK_TEXTURE_H_
#define _MAGICK_TEXTURE_H_

#include "ResourceSystem/Texture.h"
#include <Magick++.h>

class MagickTexture : public Texture
{
public:
	MagickTexture();
	MagickTexture(const char *name);
	~MagickTexture();

	virtual bool load(const char *name);
	virtual bool save(const char *name);

	virtual const void *getData();

private:
	Magick::Image mImage;
	Magick::Blob mBolb;
};

#endif