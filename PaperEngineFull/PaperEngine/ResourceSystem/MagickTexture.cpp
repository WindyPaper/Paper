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
	// ���油��
}

const void *MagickTexture::getData()
{
	return mBolb.data();
}

