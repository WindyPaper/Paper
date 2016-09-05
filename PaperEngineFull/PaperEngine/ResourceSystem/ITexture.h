#ifndef _ITEXTURE_H_
#define _ITEXTURE_H_

#include "ResourceSystem/Resource.h"
#include "ResourceSystem/ResHandle.h"

enum TexturePixelFormat
{
	UNKNOW_PIXEL = -1,
	X8R8G8B8 = 0,
	R8G8B8,
	B8G8R8,
	B8G8R8A8,
	R8G8B8A8,
	A8B8G8R8,
	A8R8G8B8,
	R5G6B5,
	A1R5G5B5,
	A4R4G4B4,
	DXT1,
	DXT2,
	DXT3,
	DXT4,
	DXT5,
	L8A8,
};

enum TextureUsage
{
	Default,
	Static,
	Dynamic,
	Stream,
	Read,
	Write,
	RenderTarget,
};

enum TextureType
{
	Texture2D,
	Texture3D,
	TextureCube,
};

enum ShaderTexType
{
	TexDiffuse = 0,
	TexNormal,
	TexSpecular,
	TexMask,
	TexDepthMap,
	TexGBNormalMap,
	TexGBWorldPosMap,
	TexGBFluxColorMap,

	ShaderTexTypeNum,
};

class ITexture : public Resource
{
public:
	ITexture() :
		mIsFlipX(false),
		mIsFlipY(false),
		mFormat(UNKNOW_PIXEL)
	{

	}

	virtual ~ITexture() {}

	virtual bool createTexture(int width, int height, TextureUsage usage, TextureType textureType, TexturePixelFormat format, void *data) = 0;
	virtual void bind(const uint texUnit) const = 0;
	virtual void setData(unsigned char *pData, const int width, const int height, TexturePixelFormat format) = 0;
	virtual int getWidth() const = 0;
	virtual int getHeight() const = 0;
	virtual bool isFlipX() const { return mIsFlipX; }
	virtual bool isFlipY() const { return mIsFlipY; }
	virtual TexturePixelFormat getFormat() const { return mFormat; }
	virtual TextureType getType() { return mTexType; }

	virtual unsigned char *lock() = 0;
	virtual void unlock() = 0;

protected:
	TextureType mTexType;
	TexturePixelFormat mFormat;
	bool mIsFlipX;
	bool mIsFlipY;
};

typedef ResHandle<ITexture*> TexHandle;

#endif