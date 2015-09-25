#include "OpenGLPlatform/RenderPreCompileHead.h"
#include "util/Log.h"
#include "OpenGLPlatform/OpenGLImpl.h"
#include "OpenGLPlatform/OpenGLTexture.h"
#include "ResourceSystem/MagicImageLoader.h"

OpenGLTexture::OpenGLTexture(const char *filename, const TextureType texType) :
mWidth(0),
mHeight(0),
mTexType(Texture2D),
mpTexMemData(0),
mTexObject(0),
mGLInternalFormat(0),
mGLFormat(0)
{
	mName = filename;

	MagicImageLoader magicImageLoader;
	//ImageRes image(filename, &magicImageLoader);

	//loadInGLBuf(image.getWidth(), image.getHeight(), image.getFormat(), image.getData(), texType);
}

OpenGLTexture::OpenGLTexture() :
mWidth(0),
mHeight(0),
mTexType(Texture2D),
mpTexMemData(0),
mTexObject(0),
mGLInternalFormat(0),
mGLFormat(0)
{

}

bool OpenGLTexture::createTexture(int width, int height, TextureUsage usage, TextureType textureType, TexturePixelFormat format, void *data)
{
	int imageByteSize = 0;

	if (textureType != Texture2D)
	{
		Log::getInstance().logMsg("Only can load Texture2D now!");
		return false;
	}

	if (format == R8G8B8)
	{
		imageByteSize = width * height * 3;
	}
	else if (format == B8G8R8A8)
	{
		imageByteSize = width * height * 4;
	}
	else if (format == L8A8)
	{
		imageByteSize = width * height * 2;
	}
	else
	{
		Log::getInstance().logMsg("Only can't load image Format now!");
		return false;
	}

	mpTexMemData = new unsigned char[imageByteSize];
	memset(mpTexMemData, 0, imageByteSize);
	
	loadInGLBuf(width, height, format, mpTexMemData, textureType);
}

bool OpenGLTexture::load(ResourceLoader *pLoader)
{
	//ImageRes image(mName.c_str(), pLoader);

	//loadInGLBuf(image.getWidth(), image.getHeight(), image.getFormat(), image.getData(), Texture2D);
	return true;
}

bool OpenGLTexture::load(const std::string &name)
{
	mName = name;

	MagicImageLoader magicImageLoader;
	//ImageRes image(name.c_str(), &magicImageLoader);
 	magicImageLoader.loadRes(this);

	//loadInGLBuf(image.getWidth(), image.getHeight(), image.getFormat(), image.getData(), Texture2D);
	return true;
}

void OpenGLTexture::unload()
{
	SAFE_DELETE(mpTexMemData);
	if (mTexObject != 0)
	{
		glDeleteTextures(1, &mTexObject);
		mTexObject = 0;
	}	
}

void OpenGLTexture::updateTexture()
{
	glBindTexture(mTexType, mTexObject);
	OpenGLImpl::getInstancePtr()->checkError();
	glTexImage2D(mTexType, 0, mGLInternalFormat, mWidth, mHeight, 0, mGLFormat, GL_UNSIGNED_BYTE, mpTexMemData);
}

void OpenGLTexture::loadInGLBuf(const uint width, const uint height, const TexturePixelFormat format, const unsigned char *buf, const TextureType texType)
{
	mWidth = width;
	mHeight = height;

	uint type = GL_INVALID_VALUE;

	switch(texType)
	{
	case Texture2D:
		type = GL_TEXTURE_2D;
		break;
	case Texture3D:
		type = GL_TEXTURE_3D;
		break;
	default:
		break;
	}

	if(type == GL_INVALID_VALUE)
	{
		//error log
		return;
	}

	uint glInternalFormat = GL_INVALID_VALUE;
	uint glFormat = GL_INVALID_VALUE;
	uint component = 4;
	switch(format)
	{
	case R8G8B8A8:
		glInternalFormat = GL_RGBA;
		glFormat = GL_BGRA;
		component = 4;
		break;
	case R8G8B8:
		glInternalFormat = GL_RGB;
		glFormat = GL_BGR;
		component = 3;
		break;
	case L8A8:
		glInternalFormat = GL_LUMINANCE_ALPHA;
		glFormat = GL_LUMINANCE_ALPHA;
		component = 2;
		break;
	default:
		break;
	}
	mTexType = type;
	mTexPixFormat = format;
	mGLInternalFormat = glInternalFormat;
	mGLFormat = glFormat;
	OpenGLImpl::getInstancePtr()->checkError();
	glEnable(mTexType);
	glActiveTexture(GL_TEXTURE0);

	// Set unpack alignment to one byte
	//int alignment = 0;
	//glGetIntegerv(GL_UNPACK_ALIGNMENT, &alignment);
	//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	OpenGLImpl::getInstancePtr()->checkError();
	if((int)mTexObject <= 0)
	{
		
		glGenTextures(1, &mTexObject);
		//OpenGLImpl::getInstance().activeTexObj(mTexType, mTexObject);
		//OpenGLImpl::getInstance().activeTexUnit(0);
		//glActiveTexture(0);
		
	}
	//glBindTexture(mTexType, mTexObject);zz
	//glActiveTexture(GL_TEXTURE0);
	//glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
	//glBindTexture(mTexType, mTexObject);
	OpenGLImpl::getInstance().activeTexObj(mTexType, mTexObject);
	OpenGLImpl::getInstancePtr()->checkError();
	glTexImage2D(mTexType, 0, glInternalFormat, width, height, 0, glFormat, GL_UNSIGNED_BYTE, buf);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameterf(mTexType, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);//mipmap
	//glTexParameterf(mTexType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(mTexType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	
	OpenGLImpl::getInstancePtr()->checkError();
	// Restore old unpack alignment
//	glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);
	glBindTexture(mTexType, 0);
	glDisable(mTexType);
}

void OpenGLTexture::bind(const uint texUnit) const
{
	//glActiveTexture(texUnit);
	glEnable(mTexType);
	OpenGLImpl::getInstancePtr()->activeTexUnit(texUnit);	
	glBindTexture(mTexType, mTexObject);
}

int OpenGLTexture::getWidth() const
{
	return mWidth;
}

int OpenGLTexture::getHeight() const
{
	return mHeight;
}

unsigned char * OpenGLTexture::lock()
{
	return mpTexMemData;
}

void OpenGLTexture::unlock()
{
	updateTexture();
}

void OpenGLTexture::setData(unsigned char *pData, const int width, const int height, TexturePixelFormat format)
{
	if (mpTexMemData)
	{
		unload();
	}

	mpTexMemData = pData;
	mTexPixFormat = format;

	loadInGLBuf(width, height, format, pData, Texture2D);
}

OpenGLTexture::~OpenGLTexture()
{
	glDeleteTextures(1, &mTexObject);
}
