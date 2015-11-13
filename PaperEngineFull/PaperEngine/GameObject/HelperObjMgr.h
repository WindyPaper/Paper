#ifndef _HELPER_OBJ_MGR_H_
#define _HELPER_OBJ_MGR_H_

#include <vector>
#include "util/Singleton.h"
#include "util/CollionGeo.h"

class IGameObject;

class HelperObjMgr : public Singleton<HelperObjMgr>
{
public:
	static HelperObjMgr &getInstance();
	static HelperObjMgr *getInstancePtr();

	HelperObjMgr();
	~HelperObjMgr();

	void addAABBBox(const AABB &aabb);

	void fillVertexBufInsData();

protected:
	void convertAABBToMatrix(math::Matrix44 *pMatrixList);

private:
	std::vector<IGameObject*> helperObjVec;

	std::vector<AABB> mCubeBoxVec;

	math::Matrix44 *mpBoxMat;
};

#endif