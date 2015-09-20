#include "util/PrecompileHead.h"
#include "ResourceSystem/MagicImageLoader.h"
#include <FreeImage.h>
#include "util/Engine_Define.h"
#include "util/Log.h"
#include "ResourceSystem/ITexture.h"

bool MagicImageLoader::loadRes(Resource *res)
{
	{
		//这段都是测试代码，一般加到纹理管理器或者资源管理器那里的
		static bool hasInit = false;
//#ifdef FREEIMAGE_LIB
		if(hasInit == false)
		{
			hasInit = true;
			FreeImage_Initialise(0);
		}
//#endif
	}

	ITexture *pImageRes = dynamic_cast<ITexture*>(res);
	if(pImageRes == 0)
	{
		return false;
	}

	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	FIBITMAP *dib = 0;
	BYTE *bits = 0;
	uint width = 0;
	uint height = 0;
	const char *pFileName = pImageRes->getName().c_str();
	fif = FreeImage_GetFileType(pFileName);
	if(fif != FIF_UNKNOWN)
	{
		//fif = FreeImage_GetFIFFromFilename(pImageRes->getFileName());
	}
	else
	{
		//Log::getInstance().logMsg("get texture error");

		fif = FreeImage_GetFIFFromFilename(pFileName);

		if (fif == FIF_UNKNOWN)
		{
			return false;
		}		
	}
	if(FreeImage_FIFSupportsReading(fif))
	{
		dib = FreeImage_Load(fif, pFileName);
		if (dib == 0)
		{
			Log::getInstance().logMsg("didn't find image file %s", pFileName);
		}
	}
	else
	{
		Log::getInstance().logMsg("Error tex format");
		return false;
	}

	//dib = FreeImage_ConvertTo32Bits(dib);
	width = FreeImage_GetWidth(dib);
	height = FreeImage_GetHeight(dib);
	bits = FreeImage_GetBits(dib);

	if(bits == 0 || width == 0 || height == 0)
	{
		Log::getInstance().logMsg("tex some thing 0");
		return false;
	}
	//pImageRes->setWidth(width);
	//pImageRes->setHeight(height);
	uint imageSize = FreeImage_GetDIBSize(dib);
	int bpp = FreeImage_GetBPP(dib);
	FREE_IMAGE_COLOR_TYPE colorType = FreeImage_GetColorType(dib);

	//FREE_IMAGE_FORMAT = FreeImage_GetFileType()
	/*if(colorType == FIC_RGB)
	{
		pImageRes->setFormat(ImagePixelFormat::R8G8B8);
	}
	else if(colorType == FIC_RGBALPHA)
	{
		pImageRes->setFormat(ImagePixelFormat::R8G8B8A8);
	}
	else if (colorType == FIF_PNG)
	{
		pImageRes->setFormat(ImagePixelFormat::R8G8B8A8);
	}
	else
	{
		Log::getInstance().logMsg("unknow image format !!");
		assert(false);
	}*/
	TexturePixelFormat pixelFormat = UNKNOW_PIXEL;
	if (fif == FIF_PNG)
	{
		pixelFormat = R8G8B8A8;
		//pImageRes->setFormat(R8G8B8A8);
		if (bpp == 24)
		{
			pixelFormat = R8G8B8;
		}
	}
	else if (fif == FIF_JPEG)
	{
		pixelFormat = R8G8B8;
		//pImageRes->setFormat(R8G8B8);
	}
	else if (fif == FIF_TARGA)
	{
		if (colorType == FIC_RGBALPHA || bpp == 32)
		{
			pixelFormat = R8G8B8A8;
		}
		else if (colorType == FIC_RGB || bpp == 24)
		{
			pixelFormat = R8G8B8;
		}
	}
	else
	{
		Log::getInstance().logMsg("unknow image format !!");
		assert(false);
	}
	
	//pImageRes->setImageData(reinterpret_cast<char*>(bits), );
	unsigned char *pNewData = new unsigned char[imageSize];
	memset(pNewData, 0, imageSize);

	unsigned char *textureData = FreeImage_GetBits(dib);

	memcpy(pNewData, FreeImage_GetBits(dib), imageSize);
	//pImageRes->setImageData(reinterpret_cast<char*>(pNewData), imageSize);
	pImageRes->setData(pNewData, width, height, pixelFormat);

	//save
	/*

	FIBITMAP* bitmap = FreeImage_Allocate(width, height, 32, 8, 8, 8);
	for (int y = 0; y < FreeImage_GetHeight(bitmap); y++)
	{
		BYTE *bits = FreeImage_GetScanLine(bitmap, y);
		for (int x = 0; x < FreeImage_GetWidth(bitmap); x++)
		{
			bits[0] = pNewData[(y * width + x) * 4 + 0];
			bits[1] = pNewData[(y * width + x) * 4 + 1];
			bits[2] = pNewData[(y * width + x) * 4 + 2];
			bits[3] = pNewData[(y * width + x) * 4 + 3];
			bits += 4;

		}
	}
	bool bSuccess = FreeImage_Save(FIF_PNG, bitmap, "D:\\save_pic.png", PNG_DEFAULT);
	FreeImage_Unload(bitmap);
	*/

	FreeImage_Unload(dib);

	return true;

	//try
	//{
	//	Magick::Image *pImage = new Magick::Image(pImageRes->getFileName());
	//	Magick::Blob blob;
	//	pImage->write(&blob, "RGBA");
	//	
	//	pImageRes->setWidth(pImage->columns());
	//	pImageRes->setHeight(pImage->rows());
	//	pImageRes->setFormat(ImagePixelFormat::R8G8B8A8);

	//	char *pNewData = new char[blob.length()];
	//	memcpy(pNewData, blob.data(), blob.length());
	//	pImageRes->setImageData(pNewData, blob.length());
	//}
	//catch(Magick::Error &error)
	//{
	//	//error log
	//	return false;
	//}
}