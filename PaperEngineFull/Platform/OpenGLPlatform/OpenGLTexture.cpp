#include "OpenGLPlatform/RenderPreCompileHead.h"
#include "util/Log.h"
#include "util/Global.h"
#include "OpenGLPlatform/OpenGLImpl.h"
#include "OpenGLPlatform/OpenGLTexture.h"
#include "ResourceSystem/MagicImageLoader.h"

OpenGLTexture::OpenGLTexture(const char *filename, const TextureType texType) :
mTexObject(0),
mGLInternalFormat(0),
mGLFormat(0),
mpTextureData(0)
{
	mName = filename;
	//mTexType = texType;

	switch (texType)
	{
	case Texture2D:
		mpTextureData = new Texture2dData();
		mTexType = GL_TEXTURE_2D;
		break;
	case Texture3D:
		break;
	case TextureCube:
		//mpTextureData = new Texture2dData();
		break;
	default:
		break;
	}
}

OpenGLTexture::OpenGLTexture() :
mTexObject(0),
mGLInternalFormat(0),
mGLFormat(0),
mpTextureData(0)
{
	mTexType = GL_TEXTURE_2D;
}

bool OpenGLTexture::createTexture(int width, int height, TextureUsage usage, TextureType textureType, TexturePixelFormat format, void *data)
{
	int imageByteSize = 0;
	mFormat = format;

	if (mpTextureData == 0)
	{
		switch (textureType)
		{
		case Texture2D:
			mpTextureData = new Texture2dData();
			break;
		case Texture3D:
			break;
		case TextureCube:
			//mpTextureData = new Texture2dData();
			break;
		default:
			break;
		}
	}

	assert(mpTextureData);

	if (textureType == Texture2D)
	{
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

		//unsigned char **pTexMemData = &dynamic_cast<Texture2dData*>(mpTextureData)->pTexMemData;
		dynamic_cast<Texture2dData*>(mpTextureData)->pTexMemData = new unsigned char[imageByteSize];
		memset(dynamic_cast<Texture2dData*>(mpTextureData)->pTexMemData, 0, imageByteSize);

		loadInGLBuf(width, height, format, dynamic_cast<Texture2dData*>(mpTextureData)->pTexMemData, textureType);
	}
}

bool OpenGLTexture::load(ResourceLoader *pLoader)
{
	//ImageRes image(mName.c_str(), pLoader);

	//loadInGLBuf(image.getWidth(), image.getHeight(), image.getFormat(), image.getData(), Texture2D);
	return true;
}

bool OpenGLTexture::load(const std::string &name)
{
	if (mpTextureData == 0)
	{
		mpTextureData = new Texture2dData();
	}

	mName = name;

	MagicImageLoader magicImageLoader;
	//ImageRes image(name.c_str(), &magicImageLoader);
	magicImageLoader.loadRes(this);

	//loadInGLBuf(image.getWidth(), image.getHeight(), image.getFormat(), image.getData(), Texture2D);
	return true;
}

bool OpenGLTexture::load(const std::string &name, const NameParamMap &paramMap)
{
	NameParamMap::const_iterator type = paramMap.find("TextureType");
	if (type->second == "CubeMap")
	{
		Texture3dData *p3dData = new Texture3dData();
		mpTextureData = p3dData;

		mTexType = GL_TEXTURE_CUBE_MAP;

		mName = name;

		ITexture *pTex[6];
		const std::string *image[6];

		for (int i = 0; i < 6; ++i)
		{
			char index_str[2];
			memset(index_str, 0, sizeof(index_str));

			sprintf_s(index_str, "%d", i);

			//std::string index = index_str;
			NameParamMap::const_iterator iter = paramMap.find(index_str);
			image[i] = &iter->second;

			NameParamMap flipPair;
			flipPair["TextureType"] = "Texture2D";
			flipPair["FlipX"] = "true";
			flipPair["FlipY"] = "true";
			pTex[i] = gEngModule->pTexMgr->create(*(image[i]), flipPair);
			p3dData->texHandle[i] = gEngModule->pTexMgr->getHandle(pTex[i]->getName());
		}

		glGenTextures(1, &mTexObject);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, mTexObject);

		OpenGLImpl::getInstance().checkError();

		for (int i = 0; i < 6; ++i)
		{
			uint glInternalFormat = GL_INVALID_VALUE;
			uint glFormat = GL_INVALID_VALUE;
			switch (pTex[i]->getFormat())
			{
			case R8G8B8A8:
				glInternalFormat = GL_RGBA;
				glFormat = GL_BGRA;
				break;
			case R8G8B8:
				glInternalFormat = GL_RGB;
				glFormat = GL_BGR;
				break;
			case L8A8:
				glInternalFormat = GL_LUMINANCE_ALPHA;
				glFormat = GL_LUMINANCE_ALPHA;
				break;
			default:
				assert(false);
				break;
			}
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, glInternalFormat, pTex[i]->getWidth(), pTex[i]->getHeight(), 0, glFormat, GL_UNSIGNED_BYTE, pTex[i]->lock());
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}
	else
	{
		if (mpTextureData == 0)
		{
			mpTextureData = new Texture2dData();
		}

		mName = name;

		NameParamMap::const_iterator type = paramMap.find("FlipX");
		if (type != paramMap.end())
		{
			if (type->second == "true")
			{
				mIsFlipX = true;
			}
			else
			{
				mIsFlipX = false;
			}
		}

		type = paramMap.find("FlipY");
		if (type != paramMap.end())
		{
			if (type->second == "true")
			{
				mIsFlipY = true;
			}
			else
			{
				mIsFlipY = false;
			}
		}

		MagicImageLoader magicImageLoader;
		//ImageRes image(name.c_str(), &magicImageLoader);
		magicImageLoader.loadRes(this);

		//loadInGLBuf(image.getWidth(), image.getHeight(), image.getFormat(), image.getData(), Texture2D);
		return true;
	}

	return true;
}

void OpenGLTexture::unload()
{
	SAFE_DELETE(mpTextureData);
	if (mTexObject != 0)
	{
		glDeleteTextures(1, &mTexObject);
		mTexObject = 0;
	}
}

void OpenGLTexture::updateTexture()
{
	int width = dynamic_cast<Texture2dData*>(mpTextureData)->width;
	int height = dynamic_cast<Texture2dData*>(mpTextureData)->height;
	glBindTexture(mTexType, mTexObject);
	OpenGLImpl::getInstancePtr()->checkError();
	glTexImage2D(mTexType, 0, mGLInternalFormat, width, height, 0, mGLFormat, GL_UNSIGNED_BYTE, dynamic_cast<Texture2dData*>(mpTextureData)->pTexMemData);
}

void OpenGLTexture::loadInGLBuf(const uint width, const uint height, const TexturePixelFormat format, const unsigned char *buf, const TextureType texType)
{
	dynamic_cast<Texture2dData*>(mpTextureData)->width = width;
	dynamic_cast<Texture2dData*>(mpTextureData)->height = height;

	uint type = GL_INVALID_VALUE;

	switch (texType)
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

	if (type == GL_INVALID_VALUE)
	{
		//error log
		return;
	}

	uint glInternalFormat = GL_INVALID_VALUE;
	uint glFormat = GL_INVALID_VALUE;
	uint component = 4;
	switch (format)
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
	dynamic_cast<Texture2dData*>(mpTextureData)->texPixFormat = format;
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
	if ((int)mTexObject <= 0)
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
	return dynamic_cast<Texture2dData*>(mpTextureData)->width;
}

int OpenGLTexture::getHeight() const
{
	return dynamic_cast<Texture2dData*>(mpTextureData)->height;
}

unsigned char * OpenGLTexture::lock()
{
	return dynamic_cast<Texture2dData*>(mpTextureData)->pTexMemData;
}

void OpenGLTexture::unlock()
{
	updateTexture();
}

void OpenGLTexture::setData(unsigned char *pData, const int width, const int height, TexturePixelFormat format)
{
	mFormat = format;
	if (dynamic_cast<Texture2dData*>(mpTextureData)->pTexMemData)
	{
		unload();
	}

	dynamic_cast<Texture2dData*>(mpTextureData)->pTexMemData = pData;
	dynamic_cast<Texture2dData*>(mpTextureData)->texPixFormat = format;

	loadInGLBuf(width, height, format, pData, Texture2D);
}

OpenGLTexture::~OpenGLTexture()
{
	glDeleteTextures(1, &mTexObject);
}
