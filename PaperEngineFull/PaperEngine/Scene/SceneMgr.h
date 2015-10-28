#ifndef _SCENE_MGR_H_
#define _SCENE_MGR_H_

#include <map>
#include <vector>
#include "util/Singleton.h"

class Node;
class SceneNode;
class MovableObject;
class Entity;
class IRenderSequence;
class Camera;

class SceneMgr : public Singleton<SceneMgr>
{
public:
	static SceneMgr& getInstance(void);
	static SceneMgr* getInstancePtr(void);

	SceneMgr();
	~SceneMgr();

	SceneNode *createSceneNode(const std::string &name);
	SceneNode *getSceneRootNode();

	void destorySceneNode(const std::string &name);
	void destorySceneNode(SceneNode *pNode);
	void destoryAllSceneNode();

	Entity *createEntity(const std::string &name, const math::Vector3f &pos, const math::Quaternion &quat);
	void destroyEntity(const std::string &name);
	void destoryAllEntity();

	void setRenderSequence(IRenderSequence *pRenderSequence);
	void prepareToRenderSequence();

	void updateSceneNode();

	Camera *createCamera(const std::string &cameraName);
	Camera *getMainCamera();
	const Camera *getMainCamera() const;
	void setMainCamera(Camera *pCamera);
	void destoryAllCamera();

protected:
	SceneNode *_createSceneNodeFast(const std::string &name);
	void _addToRenderSequenece(Node *pNode);
	void _createVisibleVector(const Camera *pCamera, Node *pNode);

private:
	SceneNode *mpRootNode;

	typedef std::map<std::string, SceneNode*> SceneNodeContain;
	SceneNodeContain mSceneNodeContain;

	typedef std::map<std::string, MovableObject*> MovableObjContain;
	MovableObjContain mMovableObjContain;

	IRenderSequence *mpRenderSequence;

	//std::vector<Node*> mVisibleNodeVec;

	typedef std::vector<Camera*> CameraVec;
	Camera *mpMainCamera;
	CameraVec mCameraVec;
};

#endif