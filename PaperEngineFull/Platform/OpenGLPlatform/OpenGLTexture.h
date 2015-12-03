#ifndef _OPENGL_TEXTURE_H_
#define  _OPENGL_TEXTURE_H_

#include "ResourceSystem/ITexture.h"
#include "util/Engine_Define.h"

struct ITextureData
{
	virtual ~ITextureData() {}

	virtual int getTexFormat() = 0;
	virtual unsigned char *getTexData() = 0;
};

struct Texture2dData : public ITextureData
{
	Texture2dData() :
	width(0),
	height(0),
	//texPixFormat(TexturePixelFormat::R8G8B8A8),
	pTexMemData(0)
	{

	}

	~Texture2dData()
	{
		SAFE_DELETE_ARRAY(pTexMemData);
	}

	virtual int getTexFormat() { return texPixFormat; }
	virtual unsigned char *getTexData() { return pTexMemData; }

	uint width;
	uint height;
	TexturePixelFormat texPixFormat;

	unsigned char *pTexMemData;
};

struct Texture3dData : public ITextureData
{
	Texture3dData()
	{

	}

	~Texture3dData()
	{
	}

	virtual int getTexFormat() { return texPixFormat; }
	virtual unsigned char *getTexData() { return 0; }

	TexturePixelFormat texPixFormat;

	TexHandle texHandle[6];
};

class ENGINE_DLL OpenGLTexture : public ITexture
{
public:
	OpenGLTexture();
	OpenGLTexture(const char *filename, const TextureType texType);

	virtual bool createTexture(int width, int height, TextureUsage usage, TextureType textureType, TexturePixelFormat format, void *data);

	virtual bool load(ResourceLoader *pLoader);
	virtual bool load(const std::string &name);
	virtual bool load(const std::string &name, const NameParamMap &paramMap);

	virtual void unload();

	virtual ~OpenGLTexture();
	virtual void bind(const uint texUnit) const;

	virtual int getWidth() const;
	virtual int getHeight() const;

	virtual unsigned char *lock();
	virtual void unlock();

	virtual void setData(unsigned char *pData, const int width, const int height, TexturePixelFormat format);

protected:
	void updateTexture();
	void loadInGLBuf(const uint width, const uint height, const TexturePixelFormat format, const unsigned char *buf, const TextureType texType);

private:
	int mGLInternalFormat;
	int mGLFormat;

	ITextureData *mpTextureData;

	uint mTexObject;
	uint mTexType;
};

#endif