/*!
	@file
	@author		Albert Semenov
	@date		05/2009
*/


#include "Precompiled.h"

#ifdef _WIN32
#include <windows.h>
//#include <gdiplus.h>
#endif

#include "BaseManager.h"

#include <GL/gl.h>
#include <GL/glu.h>

//for image loader
#include <malloc.h>
#include <FreeImage.h>

#include "util/Global.h"
#include "util/Log.h"

namespace base
{

	//ULONG_PTR gdiplusToken;

	BaseManager::BaseManager() :
		mGUI(nullptr),
		mPlatform(nullptr),
		mInfo(nullptr),
		hWnd(0),
		mExit(false),
		mResourceFileName("MyGUI_Core.xml")
	{
		
	}

	BaseManager::~BaseManager()
	{
	}

	void BaseManager::_windowResized()
	{
		RECT rect = { 0, 0, 0, 0 };
		GetClientRect(hWnd, &rect);
		int width = rect.right - rect.left;
		int height = rect.bottom - rect.top;

		//resizeRender(width, height);

		if (mPlatform)
			mPlatform->getRenderManagerPtr()->setViewSize(width, height);

		//setInputViewSize(width, height);
	}

	bool BaseManager::create(size_t winHandle)
	{
		hWnd = (HWND)winHandle;

		createGui();

		createScene();

		_windowResized();

		MyGUI::PointerManager::getInstance().setVisible(false);

		return true;
	}

	void BaseManager::run()
	{
		
	}

	void BaseManager::destroy()
	{
		destroyScene();

		destroyGui();
	}

	void BaseManager::setupResources()
	{
		MyGUI::xml::Document doc;

		if (!doc.open(std::string("resources.xml")))
			doc.getLastError();

		MyGUI::xml::ElementPtr root = doc.getRoot();
		if (root == nullptr || root->getName() != "Paths")
			return;

		MyGUI::xml::ElementEnumerator node = root->getElementEnumerator();
		while (node.next())
		{
			if (node->getName() == "Path")
			{
				bool root = false;
				if (node->findAttribute("root") != "")
				{
					root = MyGUI::utility::parseBool(node->findAttribute("root"));
					if (root) mRootMedia = node->getContent();
				}
				addResourceLocation(node->getContent(), false);
			}
		}

		addResourceLocation(getRootMedia() + "/Common/Base");
	}

	void BaseManager::createGui()
	{
		mPlatform = new MyGUI::OpenGLPlatform();
		mPlatform->initialise();

		setupResources();

		mGUI = new MyGUI::Gui();
		mGUI->initialise(mResourceFileName);

		mInfo = new diagnostic::StatisticInfo();
	}

	void BaseManager::destroyGui()
	{
		if (mGUI)
		{
			if (mInfo)
			{
				delete mInfo;
				mInfo = nullptr;
			}			

			mGUI->shutdown();
			delete mGUI;
			mGUI = nullptr;
		}

		if (mPlatform)
		{
			mPlatform->shutdown();
			delete mPlatform;
			mPlatform = nullptr;
		}
	}

	void BaseManager::prepare()
	{
	}

	void BaseManager::addResourceLocation(const std::string& _name, bool _recursive)
	{
		mPlatform->getDataManagerPtr()->addResourceLocation(_name, _recursive);
	}

	void BaseManager::updateFPS()
	{
		if (mInfo)
		{
			// calc FPS
			static MyGUI::Timer timer;
			const unsigned long interval = 1000;
			static int count_frames = 0;
			int accumulate = timer.getMilliseconds();
			if (accumulate > interval)
			{
				mInfo->change("FPS", (int)((unsigned long)count_frames * 1000 / accumulate));
				mInfo->update();

				count_frames = 0;
				timer.reset();
			}
			count_frames ++;
		}
	}

	void BaseManager::injectMouseMove(int _absx, int _absy, int _absz)
	{
		if (!mGUI)
			return;

		MyGUI::InputManager::getInstance().injectMouseMove(_absx, _absy, _absz);
	}

	void BaseManager::injectMousePress(int _absx, int _absy, MyGUI::MouseButton _id)
	{
		if (!mGUI)
			return;

		MyGUI::InputManager::getInstance().injectMousePress(_absx, _absy, _id);
	}

	void BaseManager::injectMouseRelease(int _absx, int _absy, MyGUI::MouseButton _id)
	{
		if (!mGUI)
			return;

		MyGUI::InputManager::getInstance().injectMouseRelease(_absx, _absy, _id);
	}

	void BaseManager::injectKeyPress(MyGUI::KeyCode _key, MyGUI::Char _text)
	{
		if (!mGUI)
			return;

		if (_key == MyGUI::KeyCode::Escape)
		{
			mExit = true;
			return;
		}
		

		MyGUI::InputManager::getInstance().injectKeyPress(_key, _text);
	}

	void BaseManager::injectKeyRelease(MyGUI::KeyCode _key)
	{
		if (!mGUI)
			return;

		MyGUI::InputManager::getInstance().injectKeyRelease(_key);
	}

	

	void BaseManager::drawOneFrame()
	{
		if (mPlatform)
			mPlatform->getRenderManagerPtr()->drawOneFrame();
	}

	void BaseManager::quit()
	{
		mExit = true;
	}

	const std::string& BaseManager::getRootMedia()
	{
		return mRootMedia;
	}

	void BaseManager::setResourceFilename(const std::string& _flename)
	{
		mResourceFileName = _flename;
	}

	diagnostic::StatisticInfo* BaseManager::getStatisticInfo()
	{
		return mInfo;
	}

	void BaseManager::renderUI()
	{
		//captureInput();
		updateFPS();
		drawOneFrame();
	}

	bool BaseManager::keyPressed(const OIS::KeyEvent &arg)
	{
		MyGUI::Char text = (MyGUI::Char)arg.text;
		MyGUI::KeyCode key = MyGUI::KeyCode::Enum(arg.key);

		injectKeyPress(key, text);
		return true;
	}

	bool BaseManager::keyReleased(const OIS::KeyEvent &arg)
	{
		MyGUI::KeyCode key = MyGUI::KeyCode::Enum(arg.key);

		injectKeyRelease(key);
		return true;
	}

	bool BaseManager::mouseMoved(const OIS::MouseEvent &arg)
	{
		injectMouseMove(arg.state.X.abs, arg.state.Y.abs, arg.state.Z.abs);
		return true;
	}

	bool BaseManager::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
	{
		injectMousePress(arg.state.X.abs, arg.state.Y.abs, MyGUI::MouseButton::Enum(id));
		return true;
	}

	bool BaseManager::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
	{
		injectMouseRelease(arg.state.X.abs, arg.state.Y.abs, MyGUI::MouseButton::Enum(id));
		return true;
	}

} // namespace base
