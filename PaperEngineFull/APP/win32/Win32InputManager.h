#ifndef _WIN32_INPUT_MANAGER_H_
#define _WIN32_INPUT_MANAGER_H_

#include "OIS.h"

class Win32InputManager : public OIS::KeyListener, public OIS::MouseListener
{
public:
	Win32InputManager(size_t handle);
	~Win32InputManager();

	bool createInputMng(size_t handle);
	void releaseInputMng();

	void captureInputDevice();
	void handlePushKey();

	const OIS::Keyboard* getKeyBoard() const;
	const OIS::Mouse* getMouse() const;

	bool isKeyDown(OIS::KeyCode key);

	virtual bool keyPressed(const OIS::KeyEvent &arg);
	virtual bool keyReleased(const OIS::KeyEvent &arg);

	virtual bool mouseMoved(const OIS::MouseEvent &arg);
	virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

	void addKeyListener(OIS::KeyListener *pListener);
	void addMouseListener(OIS::MouseListener *pListener);

	void updateMousePos();

private:
	OIS::InputManager *mpInputMng;
	OIS::Mouse *mpMouse;
	OIS::Keyboard *mpKeyBoard;

	std::vector<OIS::KeyListener*> mKeyListenerVec;
	std::vector<OIS::MouseListener*> mMouseListenerVec;
};

#endif