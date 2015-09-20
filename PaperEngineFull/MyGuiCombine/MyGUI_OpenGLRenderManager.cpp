/*!
	@file
	@author		George Evmenov
	@date		07/2009
*/
#include "Precompiled.h"
#include "MyGUI_OpenGLRenderManager.h"
#include "MyGUI_OpenGLTexture.h"
//#include "MyGUI_OpenGLVertexBuffer.h"
#include "UIRenderable.h"
#include "MyGUI_OpenGLDiagnostic.h"
#include "MyGUI_VertexData.h"
#include "MyGUI_Gui.h"
#include "MyGUI_Timer.h"

#include "GL/glew.h"

namespace MyGUI
{

	OpenGLRenderManager::OpenGLRenderManager() :
		mUpdate(false),
		mImageLoader(nullptr),
		mPboIsSupported(false),
		mIsInitialise(false)
	{
	}

	void OpenGLRenderManager::initialise(OpenGLImageLoader* _loader)
	{
		MYGUI_PLATFORM_ASSERT(!mIsInitialise, getClassTypeName() << " initialised twice");
		MYGUI_PLATFORM_LOG(Info, "* Initialise: " << getClassTypeName());

		mVertexFormat = VertexColourType::ColourABGR;

		mUpdate = false;
		mImageLoader = _loader;

		//glewInit();

		//mPboIsSupported = glewIsExtensionSupported("GL_EXT_pixel_buffer_object") != 0;

		MYGUI_PLATFORM_LOG(Info, getClassTypeName() << " successfully initialized");
		mIsInitialise = true;
	}

	void OpenGLRenderManager::shutdown()
	{
		MYGUI_PLATFORM_ASSERT(mIsInitialise, getClassTypeName() << " is not initialised");
		MYGUI_PLATFORM_LOG(Info, "* Shutdown: " << getClassTypeName());

		destroyAllResources();

		MYGUI_PLATFORM_LOG(Info, getClassTypeName() << " successfully shutdown");
		mIsInitialise = false;
	}

	IVertexBuffer* OpenGLRenderManager::createVertexBuffer()
	{
		return new UIRenderable();
	}

	void OpenGLRenderManager::destroyVertexBuffer(IVertexBuffer* _buffer)
	{
		delete _buffer;
	}

	void OpenGLRenderManager::doRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count)
	{
		UIRenderable* pUIRenderable = static_cast<UIRenderable*>(_buffer);
		//unsigned int buffer_id = buffer->getBufferID();
		//MYGUI_PLATFORM_ASSERT(buffer_id, "Vertex buffer is not created");

		/*unsigned int texture_id = 0;
		if (_texture)
		{
			MyGuiOpenGLTexture* texture = static_cast<MyGuiOpenGLTexture*>(_texture);
			texture_id = texture->getTextureID();
			//MYGUI_PLATFORM_ASSERT(texture_id, "Texture is not created");
		}

		glBindTexture(GL_TEXTURE_2D, texture_id);*/
		MyGuiOpenGLTexture* texture = static_cast<MyGuiOpenGLTexture*>(_texture);
		texture->bind();

		//create material
		//std::string matName = pUIRenderable->getName();
		//matName.append(" mat");
		IMaterial *pMat = gEngModule->pMaterialMgr->create("Res/mat_util/ui.mtl");
		pMat->setTexture(texture->getTexHandle(), 0);
		pUIRenderable->setMaterial(gEngModule->pMaterialMgr->getHandle("Res/mat_util/ui.mtl"));
		pUIRenderable->setRenderIndexValue(_count);
		gEngModule->pRenderSequence->pushToRenderSequence(pUIRenderable, RENDER_LAYER_UI);
		
	}

	void OpenGLRenderManager::begin()
	{
		
	}

	void OpenGLRenderManager::end()
	{
		
	}

	const RenderTargetInfo& OpenGLRenderManager::getInfo()
	{
		return mInfo;
	}

	const IntSize& OpenGLRenderManager::getViewSize() const
	{
		return mViewSize;
	}

	VertexColourType OpenGLRenderManager::getVertexFormat()
	{
		return mVertexFormat;
	}

	void OpenGLRenderManager::drawOneFrame()
	{
		Gui* gui = Gui::getInstancePtr();
		if (gui == nullptr)
			return;

		static Timer timer;
		static unsigned long last_time = timer.getMilliseconds();
		unsigned long now_time = timer.getMilliseconds();
		unsigned long time = now_time - last_time;

		onFrameEvent((float)((double)(time) / (double)1000));

		last_time = now_time;

		begin();
		onRenderToTarget(this, mUpdate);
		end();

		mUpdate = false;
	}

	void OpenGLRenderManager::setViewSize(int _width, int _height)
	{
		if (_height == 0)
			_height = 1;
		if (_width == 0)
			_width = 1;

		mViewSize.set(_width, _height);

		mInfo.maximumDepth = 1;
		mInfo.hOffset = 0;
		mInfo.vOffset = 0;
		mInfo.aspectCoef = float(mViewSize.height) / float(mViewSize.width);
		mInfo.pixScaleX = 1.0f / float(mViewSize.width);
		mInfo.pixScaleY = 1.0f / float(mViewSize.height);

		onResizeView(mViewSize);
		mUpdate = true;
	}

	bool OpenGLRenderManager::isPixelBufferObjectSupported() const
	{
		return mPboIsSupported;
	}

	ITexture* OpenGLRenderManager::createTexture(const std::string& _name)
	{
		MapTexture::const_iterator item = mTextures.find(_name);
		MYGUI_PLATFORM_ASSERT(item == mTextures.end(), "Texture '" << _name << "' already exist");

		MyGuiOpenGLTexture* texture = new MyGuiOpenGLTexture(_name, mImageLoader);
		mTextures[_name] = texture;
		return texture;
	}

	void OpenGLRenderManager::destroyTexture(ITexture* _texture)
	{
		if (_texture == nullptr)
			return;

		MapTexture::iterator item = mTextures.find(_texture->getName());
		MYGUI_PLATFORM_ASSERT(item != mTextures.end(), "Texture '" << _texture->getName() << "' not found");

		mTextures.erase(item);
		delete _texture;
	}

	ITexture* OpenGLRenderManager::getTexture(const std::string& _name)
	{
		MapTexture::const_iterator item = mTextures.find(_name);
		if (item == mTextures.end())
			return nullptr;
		return item->second;
	}

	void OpenGLRenderManager::destroyAllResources()
	{
		for (MapTexture::const_iterator item = mTextures.begin(); item != mTextures.end(); ++item)
		{
			delete item->second;
		}
		mTextures.clear();
	}

} // namespace MyGUI
