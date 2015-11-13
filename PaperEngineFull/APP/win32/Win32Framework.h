#ifndef _WIN32_FRAMEWORK_H_
#define _WIN32_FRAMEWORK_H_

#include "util/PrecompileHead.h"

//class Material;  //fix me. need delete
class Win32InputManager;
//class Camera;  //fix me. need delete
class CameraController;
class SceneMgr;
class GameObjSystem;
class OpenGLRenderSystem;
class HelperObjMgr;

class System;

class Win32Framework
{
public:
	Win32Framework();
	~Win32Framework();

	void init();

	void createTestResource(); //fix me
	void createTestMeshObject(const char *meshName);
	void createTestCameraControl();

	void createInputManager();
	void handleInputEvent();

	void run();

private:
	bool mIsLoop;

	System *mpSystem;

	Win32InputManager *mpInputManager;
	CameraController *mpCameraControl;

	SceneMgr *mpSceneMgr;
	GameObjSystem *mpGameObjSystem;
	HelperObjMgr *mpHelperObjMgr;

	OpenGLRenderSystem *mpGLRenderSystem;
};

#endif