#ifndef _GAME_OBJ_SYSTEM_H_
#define _GAME_OBJ_SYSTEM_H_

#include <map>
#include "GameObject/IGameObject.h"

struct ENGINE_DLL ExportModelHeadInfo
{
	int meshNum;
	int allVertexNum;
	int allIndexNum;

	ExportModelHeadInfo() :
		meshNum(0),
		allVertexNum(0),
		allIndexNum(0)
	{

	}
};

const std::string PLANE_OBJ_NAME = "PlaneMesh";
const std::string INS_AABB_OBJ_NAME = "INSAABBMesh";
const std::string SKY_BOX_OBJ_NAME = "SkyBoxMesh";

class ENGINE_DLL GameObjSystem : public IGameObjSystem, public Singleton<GameObjSystem>
{
public:
	enum eGameObjType
	{
		MODEL_GAME_OBJ,
		PLANE_OBJ,
		INS_AABB_OBJ,
		SKY_BOX_OBJ,
	};

	static GameObjSystem& getInstance(void);
	static GameObjSystem* getInstancePtr(void);

	GameObjSystem();
	virtual ~GameObjSystem();

	virtual IGameObject *createStaticGeoObj(const std::string &gameObjName, const std::string &modelName, const math::Vector3f &pos, const math::Quaternion &quat);
	virtual IGameObject *createPlane(const int width, const int height, const int seg, const math::Vector3f &pos, const math::Quaternion &quat);
	virtual IGameObject *createHelperAABBBox();
	virtual IGameObject *createSkyBox();
	virtual void destoryAllStaticGeoObj();

	virtual IGameObjRenderComp *createRenderComp(const std::string &entityName, const math::Vector3f &pos, const math::Quaternion &quat);

protected:
	IGameObject *_createGameObjFast(const std::string &gameObjName, const eGameObjType type);

private:
	typedef std::map<std::string, IGameObject*> ObjectMap;
	ObjectMap mGameObjMap;
};

#endif