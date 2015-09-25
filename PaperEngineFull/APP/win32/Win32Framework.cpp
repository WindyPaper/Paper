#include "win32/Win32Framework.h"
#include "util/WindowEventUtil.h"
#include "OpenGLPlatform/OpenGLRenderSystem.h"

#include "ResourceSystem/AssimpMeshLoader.h"
#include "OpenGLPlatform/OpenGLMaterialMgr.h"
#include "OpenGLPlatform/OpenGLShaderMgr.h"
#include "OpenGLPlatform/OpenGLTextureMgr.h"
#include "OpenGLPlatform/OpenGLWin32Window.h"
#include "util/Camera.h"
#include "util/Log.h"
#include "OpenGLPlatform/OpenGLTexture.h"
#include "CameraController.h"
#include "OpenGLPlatform/OpenGLMeshMgr.h"
#include "Scene/SceneMgr.h"
#include "GameObject/GameObjSystem.h"
#include "util/Global.h"
#include "util/System.h"
#include "util/RenderSequence.h"
#include "util/EngineProfile.h"
#include "util/TaskDispatcher.h"

#include "MyGuiCombine/GLUISystem.h"

#include "win32/Win32InputManager.h"
#include "OIS.h"

Win32Framework::Win32Framework() :
	mIsLoop(true),
	mpInputManager(0),
	mpCameraControl(0),
	mpSceneMgr(0),
	mpGameObjSystem(0),
	mpSystem(0)
{

}

Win32Framework::~Win32Framework()
{
	if(mpInputManager)
	{
		delete mpInputManager;
		mpInputManager = 0;
	}
	if(mpCameraControl)
	{
		delete mpCameraControl;
		mpCameraControl = 0;
	}

	SAFE_DELETE(mpGameObjSystem);
	SAFE_DELETE(mpSceneMgr);		
	SAFE_DELETE(gEngModule);
	SAFE_DELETE(mpGLRenderSystem);
	SAFE_DELETE(mpSystem);
}

void Win32Framework::init()
{
	mpSystem = new System();
	mpSystem->init();

	mpSceneMgr = new SceneMgr();
	mpGameObjSystem = new GameObjSystem();

	gEngModule = new EngineGlobalModule();
	memset(gEngModule, 0, sizeof(EngineGlobalModule));

	gEngModule->pMaterialMgr = new OpenGLMaterialMgr();
	gEngModule->pShaderMgr = new OpenGLShaderMgr();
	gEngModule->pMeshMgr = new OpenGLMeshMgr();
	gEngModule->pTexMgr = new TextureMgr();
	gEngModule->pRenderSequence = new RenderSequence();
	gEngModule->pUISystem = new GLUISystem();
	gEngModule->pProfile = new EngineProfile();
	gEngModule->pTaskDispatcher = new TaskDispatcher();

	gEngModule->pTaskDispatcher->init();

	mpGLRenderSystem = new OpenGLRenderSystem();
	mpGLRenderSystem->initRenderSystem();
	OpenGLImpl::getInstancePtr()->checkError();
	gEngModule->pUISystem->init((size_t)mpGLRenderSystem->getMainRenderWindow()->getWinHandle());

	createTestResource();
	createInputManager();

	mpInputManager->addKeyListener(dynamic_cast<OIS::KeyListener*>(gEngModule->pUISystem));
	mpInputManager->addMouseListener(dynamic_cast<OIS::MouseListener*>(gEngModule->pUISystem));

	Camera *pMainCamrea = mpSceneMgr->createCamera("MainCamera");
	pMainCamrea->setPos(math::Vector3f(0, 0, 0));
	pMainCamrea->setLookAt(math::Vector3f(0, 0, -1));
	pMainCamrea->setUp(math::Vector3f(0, 1, 0));
	pMainCamrea->setPerspective(45, WIN_WIDTH, WIN_HEIGHT, 1, 10000);

	mpSceneMgr->setRenderSequence(gEngModule->pRenderSequence);

	createTestCameraControl();
}

void Win32Framework::run()
{
	//engine 
	while(1)
	{
		if (mIsLoop == false)
		{
			break;
		}

		handleInputEvent();

		gEngModule->pTaskDispatcher->update();
		
		OpenGLRenderSystem::getInstance().beforeRender();
		
		mpSceneMgr->prepareToRenderSequence();
		mpSceneMgr->updateSceneNode();

		OpenGLRenderSystem::getInstance().renderAll();

		OpenGLRenderSystem::getInstance().swap();

		WindowEventUtil::messagePump();
	}
	//
	//WindowEventUtil::messagePump();
}

void Win32Framework::createTestResource()
{
	//createTestMeshObject("res/phoenix_ugv.md2");
	//createTestMeshObject("res/quad.obj");
	createTestMeshObject("res/model/test.model");
	//createTestMeshObject("res/model/test_anim.model");
	//createTestMeshObject("res/model/scene.model");
}

void Win32Framework::createTestMeshObject(const char *meshName)
{
	IGameObject *pObj = mpGameObjSystem->createStaticGeoObj(meshName, meshName, math::Vector3f(0, 0, 0), math::Quaternion(0, 0, 0, 1));
	IGameObject *pPlaneObj = mpGameObjSystem->createPlane(10, 10, 1000, math::Vector3f(0, 0, 0), math::Quaternion(0, 0, 0, 1));
}

void Win32Framework::createInputManager()
{
	if(mpInputManager == 0)
	{
		const OpenGLWin32Window *pMainRenderWin = OpenGLRenderSystem::getInstance().getMainRenderWindow();

		mpInputManager = new Win32InputManager((size_t)pMainRenderWin->getWinHandle());
	}
}

void Win32Framework::handleInputEvent()
{
	if(mpInputManager)
	{
		mpInputManager->captureInputDevice();

		if(mpInputManager->isKeyDown(OIS::KC_ESCAPE))
		{
			const OpenGLWin32Window *pMainRenderWin = OpenGLRenderSystem::getInstance().getMainRenderWindow();
			//SendMessage((HWND)pMainRenderWin->getWinHandle(), WM_CLOSE, 0, 0);
			mIsLoop = false;
		}
		mpInputManager->handlePushKey();
	}
}

void Win32Framework::createTestCameraControl()
{
	if(mpCameraControl == 0)
	{
		mpCameraControl = new CameraController(SceneMgr::getInstance().getMainCamera());
		mpInputManager->addKeyListener(mpCameraControl);
		mpInputManager->addMouseListener(mpCameraControl);
	}
}


