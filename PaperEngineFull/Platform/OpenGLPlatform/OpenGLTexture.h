#ifndef _OPENGL_TEXTURE_H_
#define  _OPENGL_TEXTURE_H_

#include "ResourceSystem/ITexture.h"
#include "util/Engine_Define.h"

class ENGINE_DLL OpenGLTexture : public ITexture
{
public:
	OpenGLTexture();
	OpenGLTexture(const char *filename, const TextureType texType);

	virtual bool createTexture(int width, int height, TextureUsage usage, TextureType textureType, TexturePixelFormat format, void *data);

	virtual bool load(ResourceLoader *pLoader);
	virtual bool load(const std::string &name);
	virtual bool load(const std::string &name, const NameParamMap &paramMap){ return true; }

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
	uint mTexType;
	uint mWidth;
	uint mHeight;
	TexturePixelFormat mTexPixFormat;
	int mGLInternalFormat;
	int mGLFormat;

	unsigned char *mpTexMemData;

	uint mTexObject;
};

#endif