/*!
	@file
	@author		Albert Semenov
	@date		08/2008
*/

#ifndef __BASE_MANAGER_H__
#define __BASE_MANAGER_H__

#include <MyGUI.h>
#include "Base/StatisticInfo.h"
#include "Base/InputFocusInfo.h"
#include "MyGUI_OpenGLPlatform.h"

#include "OIS.h"

#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
#	include <windows.h>
#	include <winuser.h>
#endif

namespace base
{

	class BaseManager : public OIS::KeyListener, public OIS::MouseListener
	{
	public:
		BaseManager();
		virtual ~BaseManager();

		virtual void prepare();
		bool create(size_t winHandle);
		void destroy();
		void run();
		void renderUI();
		void quit();

		const std::string& getRootMedia();
		void setResourceFilename(const std::string& _flename);
		void addResourceLocation(const std::string& _name, bool _recursive = false);

		diagnostic::StatisticInfo* getStatisticInfo();
		diagnostic::InputFocusInfo* getFocusInput();

	/*internal:*/
		void _windowResized();

		virtual void injectMouseMove(int _absx, int _absy, int _absz);
		virtual void injectMousePress(int _absx, int _absy, MyGUI::MouseButton _id);
		virtual void injectMouseRelease(int _absx, int _absy, MyGUI::MouseButton _id);
		virtual void injectKeyPress(MyGUI::KeyCode _key, MyGUI::Char _text);
		virtual void injectKeyRelease(MyGUI::KeyCode _key);

		virtual bool keyPressed(const OIS::KeyEvent &arg);
		virtual bool keyReleased(const OIS::KeyEvent &arg);

		virtual bool mouseMoved(const OIS::MouseEvent &arg);
		virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
		virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

	protected:
		virtual void createScene() { }
		virtual void destroyScene() { }

		virtual void setupResources();

	private:
		void createGui();
		void destroyGui();

		//void windowAdjustSettings(HWND hWnd, int width, int height, bool fullScreen);
		void updateFPS();

		void drawOneFrame();

	private:
		MyGUI::Gui* mGUI;
		MyGUI::OpenGLPlatform* mPlatform;
		diagnostic::StatisticInfo* mInfo;
		//diagnostic::InputFocusInfo* mFocusInfo;

		HWND hWnd;

		bool mExit;

		std::string mRootMedia;
		std::string mResourceFileName;
	};

} // namespace base

#endif // __BASE_MANAGER_H__
