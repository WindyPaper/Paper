#include "MagickTexture.h"

MagickTexture::MagickTexture()
{

}

MagickTexture::MagickTexture(const char *name)
{
	load(name);
}

bool MagickTexture::load(const char *name)
{
	try
	{
		mImage.read(name);
		mImage.write(&mBolb, "RGBA");
	}
	catch(Magick::Error &error)
	{
		//log
		//error.what();
	}
}

bool MagickTexture::save(const char *name)
{
	// ∫Û√Ê≤π…œ
}

const void *MagickTexture::getData()
{
	return mBolb.data();
}

