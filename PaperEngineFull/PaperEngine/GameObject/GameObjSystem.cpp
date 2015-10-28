#include "util/PrecompileHead.h"
#include "util/Global.h"
#include "GameObject/GameObjSystem.h"
#include "Scene/SceneMgr.h"
#include "GameObject/GameObject.h"
#include "GameObject/Entity.h"
#include "ResourceSystem/IResMgr.h"
#include "GameObject/StaticRenderable.h"
#include "GameObject/PlaneMesh.h"

GameObjSystem::GameObjSystem()
{

}

GameObjSystem::~GameObjSystem()
{
	destoryAllStaticGeoObj();
}

IGameObject * GameObjSystem::createStaticGeoObj(const std::string &gameObjName, const std::string &modelName, const math::Vector3f &pos, const math::Quaternion &quat)
{
	ObjectMap::iterator iter = mGameObjMap.find(gameObjName);
	if (iter != mGameObjMap.end())
	{
		return iter->second;
	}

	std::ifstream inStream;
	inStream.open(modelName, std::ios::in | std::ios::binary);
	inStream.seekg(0, std::ios::end);
	int fileLength = inStream.tellg();
	inStream.seekg(0, std::ios::beg);
	char *pFileCopyMem = new char[fileLength];
	memset(pFileCopyMem, 0, fileLength);
	inStream.read(pFileCopyMem, fileLength);

	char *pCurrMem = pFileCopyMem;
	ExportModelHeadInfo modelHeadInfo;
	memcpy(&modelHeadInfo, pCurrMem, sizeof(ExportModelHeadInfo));
	pCurrMem += sizeof(ExportModelHeadInfo);
	std::vector<std::string> meshNameVec;
	meshNameVec.resize(modelHeadInfo.meshNum);
	for (int i = 0; i < modelHeadInfo.meshNum; ++i)
	{
		while (*pCurrMem != '\0')
		{
			meshNameVec[i].append(1, *pCurrMem);
			++pCurrMem;
		}
		++pCurrMem;
	}

	IGameObject *pGameObj = _createGameObjFast(gameObjName, MODEL_GAME_OBJ);
	IGameObjRenderComp *pMainRenderComp = createRenderComp(gameObjName, pos, quat);
	pGameObj->setObjectComp(pMainRenderComp);

	std::string meshName = "res/mesh/" + meshNameVec[0] + ".mesh";
	IMesh *pMesh = gEngModule->pMeshMgr->create(meshName);
	pMesh->getSignalRef().connect(pMainRenderComp, &IGameObjRenderComp::updateRenderable);

	for (int i = 1; i < meshNameVec.size(); ++i)
	{
		IGameObjRenderComp *pChildEntity = createRenderComp("res/mesh/" + meshNameVec[i] + ".mesh", math::Vector3f(0.0, 0.0, 0.0), math::Quaternion(0.0, 0.0, 0.0, 1.0));
		std::string meshName = "res/mesh/" + meshNameVec[i] + ".mesh";
		IMesh *pMesh = gEngModule->pMeshMgr->create(meshName);
		
		pMesh->getSignalRef().connect(pChildEntity, &IGameObjRenderComp::updateRenderable);

		dynamic_cast<Entity*>(pMainRenderComp)->addChildEntity(dynamic_cast<Entity*>(pChildEntity));
	}

	SAFE_DELETE_ARRAY(pFileCopyMem);
	inStream.close();

	return pGameObj;
}

IGameObject * GameObjSystem::createPlane(const int width, const int height, const int seg, const math::Vector3f &pos, const math::Quaternion &quat)
{
	IGameObject *pObj = _createGameObjFast("PlaneMesh", PLANE_OBJ);
	IGameObjRenderComp *pMainRenderComp = createRenderComp("PlaneMesh", pos, quat);
	pObj->setObjectComp(pMainRenderComp);
	PlaneMesh *pPlane = dynamic_cast<PlaneMesh*>(pObj);
	pPlane->setPlaneSize(width, height, seg);
	return pPlane;
}

void GameObjSystem::destoryAllStaticGeoObj()
{
	for (ObjectMap::iterator iter = mGameObjMap.begin(); iter != mGameObjMap.end(); ++iter)
	{
		SAFE_DELETE(iter->second);
	}
	mGameObjMap.clear();
}

IGameObjRenderComp * GameObjSystem::createRenderComp(const std::string &entityName, const math::Vector3f &pos, const math::Quaternion &quat)
{
	IGameObjRenderComp *pGameObjComp = SceneMgr::getInstance().createEntity(entityName, pos, quat);
	return pGameObjComp;
}

IGameObject * GameObjSystem::_createGameObjFast(const std::string &gameObjName, const eGameObjType type)
{
	IGameObject *pObj = 0;
	switch (type)
	{
	case MODEL_GAME_OBJ:
		pObj = new GameObject(gameObjName);
		mGameObjMap[gameObjName] = pObj;
		break;
	case PLANE_OBJ:
		pObj = new PlaneMesh(gameObjName);
		mGameObjMap[gameObjName] = pObj;
	default:
		break;
	}
	
	return pObj;
}
