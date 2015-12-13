#ifndef _OPENGL_CUBE_MAP_H_
#define _OPENGL_CUBE_MAP_H_

#include <assert.h>
#include "ResourceSystem/ITexture.h"

class OpenGLCubeMap : public ITexture
{
	OpenGLCubeMap();
	~OpenGLCubeMap();

	virtual bool createTexture(int width, int height, TextureUsage usage, TextureType textureType, TexturePixelFormat format, void *data) { assert(false); return false; }
	virtual void bind(const uint texUnit) const;
	virtual void setData(unsigned char *pData, const int width, const int height, TexturePixelFormat format) { assert(false); }
	virtual int getWidth() const { return 0; }
	virtual int getHeight() const { return 0; }

	virtual bool load(ResourceLoader *pLoader);
	virtual bool load(const std::string &name);
	virtual bool load(const std::string &name, const NameParamMap &paramMap);
	virtual void unload();

	virtual unsigned char *lock();
	virtual void unlock();

private:
	//unsigned char *mpTexMemData[6];
	TexHandle mTexhandle[6];
	GLuint mCubeMapHandle;

};

#endif