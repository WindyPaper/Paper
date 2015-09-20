/*!
	@file
	@author		George Evmenov
	@date		07/2009
*/

#ifndef __MYGUI_OPENGL_TEXTURE_H__
#define __MYGUI_OPENGL_TEXTURE_H__

#include "util/Global.h"
#include "ResourceSystem/ITexture.h"
#include "MyGUI_Prerequest.h"
#include "MyGUI_ITexture.h"
#include "MyGUI_RenderFormat.h"
#include "MyGUI_OpenGLImageLoader.h"

namespace MyGUI
{
	class OpenGLRTTexture;
}


class MyGuiOpenGLTexture : public MyGUI::ITexture
{
public:
	MyGuiOpenGLTexture(const std::string& _name, MyGUI::OpenGLImageLoader* _loader);
	virtual ~MyGuiOpenGLTexture();

	virtual const std::string& getName() const;

	virtual void createManual(int _width, int _height, MyGUI::TextureUsage _usage, MyGUI::PixelFormat _format);
	virtual void loadFromFile(const std::string& _filename);
	virtual void saveToFile(const std::string& _filename);

	virtual void destroy();

	virtual int getWidth()
	{
		return gEngModule->pTexMgr->getDataPtr(mTexHandle)->getWidth();
	}
	virtual int getHeight()
	{
		return gEngModule->pTexMgr->getDataPtr(mTexHandle)->getHeight();
	}

	virtual void* lock(MyGUI::TextureUsage _access);
	virtual void unlock();
	virtual bool isLocked()
	{
		return mLock;
	}
	virtual void bind();

	virtual MyGUI::PixelFormat getFormat()
	{
		return mOriginalFormat;
	}
	virtual MyGUI::TextureUsage getUsage()
	{
		return mOriginalUsage;
	}
	virtual size_t getNumElemBytes()
	{
		return mNumElemBytes;
	}

	virtual MyGUI::IRenderTarget* getRenderTarget();

/*internal:*/
	unsigned int getTextureID() const;
	void setUsage(MyGUI::TextureUsage _usage);
	void createManual(int _width, int _height, MyGUI::TextureUsage _usage, MyGUI::PixelFormat _format, void* _data);

	//add
	TexHandle getTexHandle() const { return mTexHandle; }

private:
	void _create();

private:
	std::string mName;
	//int mWidth;
	//int mHeight;
	//int mPixelFormat;
	//int mInternalPixelFormat;
	int mUsage;
	int mAccess;
	size_t mNumElemBytes;
	size_t mDataSize;
	unsigned int mTextureID;
	unsigned int mPboID;
	bool mLock;
	//void* mBuffer;
	MyGUI::PixelFormat mOriginalFormat;
	MyGUI::TextureUsage mOriginalUsage;
	MyGUI::OpenGLImageLoader* mImageLoader;
	MyGUI::OpenGLRTTexture* mRenderTarget;
	TexHandle mTexHandle;
};


#endif // __MYGUI_OPENGL_TEXTURE_H__
