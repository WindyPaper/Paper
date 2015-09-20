#include "util/PrecompileHead.h"
#include "util/Engine_Define.h"
#include "util/Camera.h"
#include "Scene/SceneMgr.h"
#include "Scene/SceneNode.h"
#include "GameObject/Entity.h"
#include "util/RenderSequence.h"

template <> SceneMgr *Singleton<SceneMgr>::msInstance = 0;

SceneMgr *SceneMgr::getInstancePtr()
{
	return msInstance;
}

SceneMgr &SceneMgr::getInstance()
{
	assert(msInstance);
	return (*msInstance);
}

SceneMgr::SceneMgr() :
mpRootNode(0),
mpRenderSequence(0),
mpMainCamera(0)
{

}

SceneMgr::~SceneMgr()
{
	destoryAllSceneNode();

	destoryAllEntity();

	destoryAllCamera();
}

SceneNode * SceneMgr::createSceneNode(const std::string &name)
{
	SceneNodeContain::iterator iter = mSceneNodeContain.find(name);
	if (iter != mSceneNodeContain.end())
	{
		return iter->second;
	}
	else
	{
		return _createSceneNodeFast(name);
	}
}

SceneNode * SceneMgr::getSceneRootNode()
{
	if (mpRootNode == 0)
	{
		mpRootNode = createSceneNode("RootNode");
	}
	return mpRootNode;
}

void SceneMgr::destorySceneNode(const std::string &name)
{
	//TODO: here should delete child node
	SceneNodeContain::iterator iter = mSceneNodeContain.find(name);
	if (iter != mSceneNodeContain.end())
	{
		SAFE_DELETE(iter->second);
		mSceneNodeContain.erase(iter);
	}
}

void SceneMgr::destorySceneNode(SceneNode *pNode)
{
	destorySceneNode(pNode->getName());
}

void SceneMgr::destoryAllSceneNode()
{
	for (SceneNodeContain::iterator iter = mSceneNodeContain.begin(); iter != mSceneNodeContain.end(); ++iter)
	{
		SAFE_DELETE(iter->second);
	}
	mSceneNodeContain.clear();
}

Entity * SceneMgr::createEntity(const std::string &name, const math::Vector3f &pos, const math::Quaternion &quat)
{
	Entity *pEnt = new Entity(name);

	Node *pNode = createSceneNode(name);
	getSceneRootNode()->addChild(pNode);
	pEnt->setParentNode(pNode);
	pNode->setPosition(pos);
	pNode->setOrientation(quat);

	SceneNode *pSceneNode = dynamic_cast<SceneNode*>(pNode);
	pSceneNode->attachMovable(pEnt);

	mMovableObjContain[name] = pEnt;
	return pEnt;
}

void SceneMgr::destroyEntity(const std::string &name)
{
	MovableObjContain::iterator iter = mMovableObjContain.find(name);
	if (iter != mMovableObjContain.end())
	{
		SAFE_DELETE(iter->second);
		mMovableObjContain.erase(iter);
	}
}

void SceneMgr::destoryAllEntity()
{
	for (MovableObjContain::iterator iter = mMovableObjContain.begin(); iter != mMovableObjContain.end(); ++iter)
	{
		SAFE_DELETE(iter->second);
	}
	mMovableObjContain.clear();
}

void SceneMgr::setRenderSequence(IRenderSequence *pRenderSequence)
{
	mpRenderSequence = pRenderSequence;
}

void SceneMgr::prepareToRenderSequence()
{
	mpRenderSequence->clear();
	Node *pRoot = getSceneRootNode();
	_addToRenderSequenece(pRoot);
}

void SceneMgr::updateSceneNode()
{
	mpRootNode->update();
}

Camera * SceneMgr::createCamera(const std::string &cameraName)
{
	Camera *pCamera = new Camera(cameraName);
	mCameraVec.push_back(pCamera);

	if (mpMainCamera == 0)
	{
		mpMainCamera = pCamera;
	}

	return pCamera;
}

Camera * SceneMgr::getMainCamera()
{
	return mpMainCamera;
}

const Camera * SceneMgr::getMainCamera() const
{
	return mpMainCamera;
}

void SceneMgr::setMainCamera(Camera *pCamera)
{
	mpMainCamera = pCamera;
}

void SceneMgr::destoryAllCamera()
{
	for (int i = 0; i < mCameraVec.size(); ++i)
	{
		delete mCameraVec[i];
	}
	mCameraVec.clear();
}

SceneNode * SceneMgr::_createSceneNodeFast(const std::string &name)
{
	SceneNode *pNode = new SceneNode(name);
	
	mSceneNodeContain[name] = pNode;
	return pNode;
}

void SceneMgr::_addToRenderSequenece(Node *pNode)
{
	SceneNode *pSceneNode = dynamic_cast<SceneNode*>(pNode);
	int movableCount = pSceneNode->getMovableCount();
	for (int i = 0; i < movableCount; ++i)
	{
		MovableObject *pObj = pSceneNode->getMovable(i);
		pObj->pushToRenderSequence(mpRenderSequence);
	}

	int count = pNode->getChildCount();
	for (int i = 0; i < count; ++i)
	{
		Node *pChildNode = pNode->getChild(i);
		_addToRenderSequenece(pChildNode);
	}
}
