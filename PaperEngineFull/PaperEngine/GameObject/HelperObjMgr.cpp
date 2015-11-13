#include "util/PrecompileHead.h"
#include "util/Engine_Define.h"
#include "GameObject/HelperObjMgr.h"
#include "GameObject/GameObjSystem.h"
#include "GameObject/BoundInsMesh.h"

template<> HelperObjMgr *Singleton<HelperObjMgr>::msInstance = 0;

HelperObjMgr & HelperObjMgr::getInstance()
{
	assert(msInstance);
	return (*msInstance);
}

HelperObjMgr * HelperObjMgr::getInstancePtr()
{
	return msInstance;
}

HelperObjMgr::HelperObjMgr() :
mpBoxMat(0)
{
	IGameObject *pObj = GameObjSystem::getInstance().createHelperAABBBox();
	dynamic_cast<BoundInsMesh*>(pObj)->initCubeMesh();
	helperObjVec.push_back(pObj);

	mpBoxMat = new math::Matrix44[MAX_INSTANCE_NUM];
	memset(mpBoxMat, 0, sizeof(math::Matrix44) * MAX_INSTANCE_NUM);
}

HelperObjMgr::~HelperObjMgr()
{
	SAFE_DELETE_ARRAY(mpBoxMat);
}

void HelperObjMgr::addAABBBox(const AABB &aabb)
{
	mCubeBoxVec.push_back(aabb);
}

void HelperObjMgr::fillVertexBufInsData()
{
	for (int i = 0; i < helperObjVec.size(); ++i)
	{
		IGameObject *pObj = helperObjVec[i];
		if (pObj->getName() == INS_AABB_OBJ_NAME) //fix me
		{
			BoundInsMesh *pBoundInsMesh = dynamic_cast<BoundInsMesh*>(pObj);

			convertAABBToMatrix(mpBoxMat);

			pBoundInsMesh->getMesh()->bindInstanceData(\
				reinterpret_cast<unsigned char*>(mpBoxMat), sizeof(math::Matrix44), mCubeBoxVec.size(), InstanceDataSortType::MVP_WORLD_TYPE);
		}
	}

	mCubeBoxVec.clear();
}

void HelperObjMgr::convertAABBToMatrix(math::Matrix44 *pMatrixList)
{
	math::Matrix44 *pCurrData = pMatrixList;

	assert(mCubeBoxVec.size() < MAX_INSTANCE_NUM);

	for (int i = 0; i < mCubeBoxVec.size(); ++i)
	{
		AABB &aabb = mCubeBoxVec[i];

		math::Vector3f pos = aabb.getPos();
		math::Vector3f &scale = math::Vector3f(aabb.max.x - aabb.min.x, aabb.max.y - aabb.min.y, aabb.max.z - aabb.min.z);
		pCurrData->initFullTransform(pos, \
			scale, \
			math::QUAT_IDENTIFY);

		pCurrData->transpose();

		++pCurrData;
	}
}
