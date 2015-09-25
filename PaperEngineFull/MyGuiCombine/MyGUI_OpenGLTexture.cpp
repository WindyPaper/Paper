/*!
	@file
	@author		George Evmenov
	@date		07/2009
*/
#include "Precompiled.h"
#include "MyGUI_OpenGLTexture.h"
#include "MyGUI_OpenGLRenderManager.h"
#include "MyGUI_OpenGLDiagnostic.h"
#include "MyGUI_OpenGLPlatform.h"
#include "MyGUI_OpenGLRTTexture.h"

#include "GL/glew.h"
#include "OpenGLPlatform/OpenGLImpl.h"


MyGuiOpenGLTexture::MyGuiOpenGLTexture(const std::string& _name, MyGUI::OpenGLImageLoader* _loader) :
	mName(_name),
	mTextureID(0),
	mPboID(0),
	//mWidth(0),
	//mHeight(0),
	mLock(false),
	//mPixelFormat(0),
	mDataSize(0),
	mUsage(0),
	//mBuffer(0),
	//mInternalPixelFormat(0),
	mAccess(0),
	mNumElemBytes(0),
	mImageLoader(_loader),
	mRenderTarget(nullptr)
{
}

MyGuiOpenGLTexture::~MyGuiOpenGLTexture()
{
	destroy();
}

const std::string& MyGuiOpenGLTexture::getName() const
{
	return mName;
}

void MyGuiOpenGLTexture::setUsage(MyGUI::TextureUsage _usage)
{
	mAccess = 0;
	mUsage = 0;

	if (_usage == MyGUI::TextureUsage::Default)
	{
		mUsage = GL_STATIC_READ_ARB;
		mAccess = GL_READ_ONLY_ARB;
	}
	else if (_usage.isValue(MyGUI::TextureUsage::Static))
	{
		if (_usage.isValue(MyGUI::TextureUsage::Read))
		{
			if (_usage.isValue(MyGUI::TextureUsage::Write))
			{
				mUsage = GL_STATIC_COPY_ARB;
				mAccess = GL_READ_WRITE_ARB;
			}
			else
			{
				mUsage = GL_STATIC_READ_ARB;
				mAccess = GL_READ_ONLY_ARB;
			}
		}
		else if (_usage.isValue(MyGUI::TextureUsage::Write))
		{
			mUsage = GL_STATIC_DRAW_ARB;
			mAccess = GL_WRITE_ONLY_ARB;
		}
	}
	else if (_usage.isValue(MyGUI::TextureUsage::Dynamic))
	{
		if (_usage.isValue(MyGUI::TextureUsage::Read))
		{
			if (_usage.isValue(MyGUI::TextureUsage::Write))
			{
				mUsage = GL_DYNAMIC_COPY_ARB;
				mAccess = GL_READ_WRITE_ARB;
			}
			else
			{
				mUsage = GL_DYNAMIC_READ_ARB;
				mAccess = GL_READ_ONLY_ARB;
			}
		}
		else if (_usage.isValue(MyGUI::TextureUsage::Write))
		{
			mUsage = GL_DYNAMIC_DRAW_ARB;
			mAccess = GL_WRITE_ONLY_ARB;
		}
	}
	else if (_usage.isValue(MyGUI::TextureUsage::Stream))
	{
		if (_usage.isValue(MyGUI::TextureUsage::Read))
		{
			if (_usage.isValue(MyGUI::TextureUsage::Write))
			{
				mUsage = GL_STREAM_COPY_ARB;
				mAccess = GL_READ_WRITE_ARB;
			}
			else
			{
				mUsage = GL_STREAM_READ_ARB;
				mAccess = GL_READ_ONLY_ARB;
			}
		}
		else if (_usage.isValue(MyGUI::TextureUsage::Write))
		{
			mUsage = GL_STREAM_DRAW_ARB;
			mAccess = GL_WRITE_ONLY_ARB;
		}
	}
}

void MyGuiOpenGLTexture::createManual(int _width, int _height, MyGUI::TextureUsage _usage, MyGUI::PixelFormat _format)
{
	OpenGLImpl::getInstancePtr()->checkError();
	createManual(_width, _height, _usage, _format, 0);
}

void MyGuiOpenGLTexture::createManual(int _width, int _height, MyGUI::TextureUsage _usage, MyGUI::PixelFormat _format, void* _data)
{
	MYGUI_PLATFORM_ASSERT(mTexHandle.isNull(), "Texture already exist");

	//FIXME перенести в метод
	//mInternalPixelFormat = 0;
	//mPixelFormat = 0;
	mNumElemBytes = 0;
	TexturePixelFormat format;
	if (_format == MyGUI::PixelFormat::L8)
	{
		//mInternalPixelFormat = GL_LUMINANCE8;
		//mPixelFormat = GL_LUMINANCE;
		//mNumElemBytes = 1;
		format = UNKNOW_PIXEL;
	}
	else if (_format == MyGUI::PixelFormat::L8A8)
	{
		//mInternalPixelFormat = GL_LUMINANCE8_ALPHA8;
		//mPixelFormat = GL_LUMINANCE_ALPHA;
		//mNumElemBytes = 2;
		format = L8A8;
	}
	else if (_format == MyGUI::PixelFormat::R8G8B8)
	{
		//mInternalPixelFormat = GL_RGB8;
		//mPixelFormat = GL_BGR;
		//mNumElemBytes = 3;
		format = R8G8B8;
	}
	else if (_format == MyGUI::PixelFormat::R8G8B8A8)
	{
		//mInternalPixelFormat = GL_RGBA8;
		//mPixelFormat = GL_BGRA;
		//mNumElemBytes = 4;
		format = R8G8B8A8;
	}
	else
	{
		MYGUI_PLATFORM_EXCEPT("format not support");
	}

	TextureUsage texUsage = Default;
	
	if (mTexHandle.isNull() == true)
	{
		gEngModule->pTexMgr->create(mName);
		mTexHandle = gEngModule->pTexMgr->getHandle(mName);
	}
	gEngModule->pTexMgr->getDataPtr(mTexHandle)->createTexture(_width, _height, texUsage, Texture2D, format, _data);
	
}

void MyGuiOpenGLTexture::destroy()
{
	if (mRenderTarget != nullptr)
	{
		delete mRenderTarget;
		mRenderTarget = nullptr;
	}

	if (mTextureID != 0)
	{
		glDeleteTextures(1, &mTextureID);
		mTextureID = 0;
	}
	if (mPboID != 0)
	{
		glDeleteBuffersARB(1, &mPboID);
		mPboID = 0;
	}

	//mWidth = 0;
	//mHeight = 0;
	mLock = false;
	//mPixelFormat = 0;
	mDataSize = 0;
	mUsage = 0;
	//mBuffer = 0;
	//mInternalPixelFormat = 0;
	mAccess = 0;
	mNumElemBytes = 0;
	//mOriginalFormat = PixelFormat::Unknow;
	//mOriginalUsage = TextureUsage::Default;
}

void* MyGuiOpenGLTexture::lock(MyGUI::TextureUsage _access)
{
	MYGUI_PLATFORM_ASSERT(!mTexHandle.isNull(), "Texture is not created");
	
	mLock = true;
	return gEngModule->pTexMgr->getDataPtr(mTexHandle)->lock();
	/*if (_access == TextureUsage::Read)
	{
		glBindTexture(GL_TEXTURE_2D, mTextureID);

		mBuffer = new unsigned char[mDataSize];
		glGetTexImage(GL_TEXTURE_2D, 0, mPixelFormat, GL_UNSIGNED_BYTE, mBuffer);

		mLock = false;

		return mBuffer;
	}

	// bind the texture
	glBindTexture(GL_TEXTURE_2D, mTextureID);
	if (!OpenGLRenderManager::getInstance().isPixelBufferObjectSupported())
	{
		//Fallback if PBO's are not supported
		mBuffer = new unsigned char[mDataSize];
	}
	else
	{
		// bind the PBO
		glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, mPboID);
			
		// Note that glMapBufferARB() causes sync issue.
		// If GPU is working with this buffer, glMapBufferARB() will wait(stall)
		// until GPU to finish its job. To avoid waiting (idle), you can call
		// first glBufferDataARB() with NULL pointer before glMapBufferARB().
		// If you do that, the previous data in PBO will be discarded and
		// glMapBufferARB() returns a new allocated pointer immediately
		// even if GPU is still working with the previous data.
		glBufferDataARB(GL_PIXEL_UNPACK_BUFFER_ARB, mDataSize, 0, mUsage);

		// map the buffer object into client's memory
		mBuffer = (GLubyte*)glMapBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, mAccess);
		if (!mBuffer)
		{
			glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
			glBindTexture(GL_TEXTURE_2D, 0);
			MYGUI_PLATFORM_EXCEPT("Error texture lock");
		}
	}*/
}

void MyGuiOpenGLTexture::unlock()
{
	/*if (!mLock && mBuffer)
	{
		delete mBuffer;
		mBuffer = 0;

		glBindTexture(GL_TEXTURE_2D, 0);

		return;
	}

	MYGUI_PLATFORM_ASSERT(mLock, "Texture is not locked");

	if (!OpenGLRenderManager::getInstance().isPixelBufferObjectSupported())
	{
		//Fallback if PBO's are not supported
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, mWidth, mHeight, mPixelFormat, GL_UNSIGNED_BYTE, mBuffer);
		delete mBuffer;
	}
	else
	{
		// release the mapped buffer
		glUnmapBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB);

		// copy pixels from PBO to texture object
		// Use offset instead of ponter.
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, mWidth, mHeight, mPixelFormat, GL_UNSIGNED_BYTE, 0);

		// it is good idea to release PBOs with ID 0 after use.
		// Once bound with 0, all pixel operations are back to normal ways.
		glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
	}
		
	glBindTexture(GL_TEXTURE_2D, 0);
	mBuffer = 0;
	mLock = false;*/
	gEngModule->pTexMgr->getDataPtr(mTexHandle)->unlock();
}

void MyGuiOpenGLTexture::loadFromFile(const std::string& _filename)
{
	destroy();

	std::string fullName = MyGUI::OpenGLDataManager::getInstance().getDataPath(_filename);
	if (mTexHandle.isNull() == true)
	{
		gEngModule->pTexMgr->create(mName);
		mTexHandle = gEngModule->pTexMgr->getHandle(mName);
	}
	gEngModule->pTexMgr->getDataPtr(mTexHandle)->load(fullName);
}

void MyGuiOpenGLTexture::saveToFile(const std::string& _filename)
{
	
}

MyGUI::IRenderTarget* MyGuiOpenGLTexture::getRenderTarget()
{
	if (mRenderTarget == nullptr)
		mRenderTarget = new MyGUI::OpenGLRTTexture(mTextureID);

	return mRenderTarget;
}

unsigned int MyGuiOpenGLTexture::getTextureID() const
{
	return mTextureID;
}

void MyGuiOpenGLTexture::bind()
{
	gEngModule->pTexMgr->getDataPtr(mTexHandle)->bind(0);
}

