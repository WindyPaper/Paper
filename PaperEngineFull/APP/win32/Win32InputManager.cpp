#include <sstream>
#include "OIS.h"
#include "util/Log.h"
#include "util/Global.h"
#include "win32/Win32InputManager.h"
#include "OpenGLPlatform/OpenGLRenderSystem.h"
#include "OpenGLPlatform/OpenGLWin32Window.h"

#include "MyGuiCombine/GLUISystem.h"



Win32InputManager::Win32InputManager(size_t handle) :
	mpInputMng(0),
	mpMouse(0),
	mpKeyBoard(0)
{
	createInputMng(handle);
}

Win32InputManager::~Win32InputManager()
{
	releaseInputMng();
}

bool Win32InputManager::createInputMng(size_t handle)
{
	bool isSuccessFul = false;
	if(mpInputMng == 0)
	{
		OIS::ParamList paramList;
		paramList.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND" )));
		paramList.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));
		paramList.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_FOREGROUND")));
		paramList.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_NONEXCLUSIVE")));
		std::ostringstream windowHndStr;
		windowHndStr<<handle;
		paramList.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

		mpInputMng = OIS::InputManager::createInputSystem(paramList);
		mpKeyBoard = static_cast<OIS::Keyboard*>(mpInputMng->createInputObject(OIS::OISKeyboard, true));
		mpKeyBoard->setEventCallback(this);

		mpMouse = static_cast<OIS::Mouse*>(mpInputMng->createInputObject(OIS::OISMouse, true));
		mpMouse->getMouseState().width = OpenGLRenderSystem::getInstancePtr()->getMainRenderWindow()->getWidth();
		mpMouse->getMouseState().height = OpenGLRenderSystem::getInstancePtr()->getMainRenderWindow()->getHeight();
		mpMouse->setEventCallback(this);

		isSuccessFul = true;
	}
	else
	{
		Log::getInstancePtr()->logMsg("The Input System Is Exsit!!!");
	}
	return isSuccessFul;
}

void Win32InputManager::releaseInputMng()
{
	if(mpInputMng)
	{
		OIS::InputManager::destroyInputSystem(mpInputMng);
		mpInputMng = 0;

		mpKeyBoard = 0;
		mpMouse = 0;
	}
}

const OIS::Keyboard* Win32InputManager::getKeyBoard() const
{
	return mpKeyBoard;
}

const OIS::Mouse* Win32InputManager::getMouse() const
{
	return mpMouse;
}

void Win32InputManager::captureInputDevice()
{
	if (mpMouse)
	{
		mpMouse->capture();
	}
	if(mpKeyBoard)
	{
		mpKeyBoard->capture();
	}
}

bool Win32InputManager::isKeyDown(OIS::KeyCode key)
{
	return mpKeyBoard->isKeyDown(key);
}

bool Win32InputManager::keyPressed(const OIS::KeyEvent &arg)
{
	OIS::KeyCode key = arg.key;
	if (key == OIS::KC_ESCAPE)
	{
		//const OpenGLWin32Window *pMainRenderWin = OpenGLRenderSystem::getInstance().getMainRenderWindow();
		//SendMessage((HWND)pMainRenderWin->getWinHandle(), WM_CLOSE, 0, 0);
		//mIsLoop = false;
		return false;
	}

	for (int i = 0; i < mKeyListenerVec.size(); ++i)
	{
		if (!mKeyListenerVec[i]->keyPressed(arg))
		{
			break;
		}
	}

	return true;
}

bool Win32InputManager::keyReleased(const OIS::KeyEvent &arg)
{
	for (int i = 0; i < mKeyListenerVec.size(); ++i)
	{
		if (!mKeyListenerVec[i]->keyReleased(arg))
		{
			break;
		}
	}

	return true;
}

void Win32InputManager::addKeyListener(OIS::KeyListener *pListener)
{
	mKeyListenerVec.push_back(pListener);
}

void Win32InputManager::addMouseListener(OIS::MouseListener *pListener)
{
	mMouseListenerVec.push_back(pListener);
}

bool Win32InputManager::mouseMoved(const OIS::MouseEvent &arg)
{
	for (int i = 0; i < mMouseListenerVec.size(); ++i)
	{
		if (!mMouseListenerVec[i]->mouseMoved(arg))
		{
			break;
		}
	}

	return true;
}

bool Win32InputManager::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	for (int i = 0; i < mMouseListenerVec.size(); ++i)
	{
		if (!mMouseListenerVec[i]->mousePressed(arg, id))
		{
			break;
		}
	}

	return true;
}

bool Win32InputManager::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	for (int i = 0; i < mMouseListenerVec.size(); ++i)
	{
		if (!mMouseListenerVec[i]->mouseReleased(arg, id))
		{
			break;
		}
	}

	return true;
}

void Win32InputManager::handlePushKey()
{
	for (uint code = OIS::KC_UNASSIGNED; code <= OIS::KC_MEDIASELECT; ++code)
	{
		if (isKeyDown((OIS::KeyCode)code))
		{
			for (int i = 0; i < mKeyListenerVec.size(); ++i)
			{
				OIS::KeyEvent keyEvt(mpKeyBoard, (OIS::KeyCode)code, 0);
				if (!mKeyListenerVec[i]->keyPressed(keyEvt))
				{
					break;
				}
			}
		}
	}
}
