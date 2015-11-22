#include "util/PrecompileHead.h"
#include "util/Engine_Define.h"
#include "util/Global.h"
#include "GameObject/BoundInsMesh.h"
#include "GameObject/StaticRenderable.h"
#include "GameObject/GameObjSystem.h"

BoundInsMesh::BoundInsMesh() :
mpCubeBoxVData(0),
mpCubeBoxIData(0)
{
	//fix me. need gennerate name
	mName = INS_AABB_OBJ_NAME;
	//_initCubeMesh();
}

BoundInsMesh::BoundInsMesh(const std::string &name) :
mpCubeBoxVData(0),
mpCubeBoxIData(0)
{
	mName = name;
	//_initCubeMesh();
}

BoundInsMesh::~BoundInsMesh()
{
	for (int i = 0; i < mMatrixVec.size(); ++i)
	{
		SAFE_DELETE(mMatrixVec[i]);
	}

	SAFE_DELETE_ARRAY(mpCubeBoxVData);
	SAFE_DELETE_ARRAY(mpCubeBoxIData);
}

void BoundInsMesh::addTransformMatrix(const math::Matrix44 matrix)
{
	math::Matrix44 *pMatrix = new math::Matrix44(matrix);
	mMatrixVec.push_back(pMatrix);
}

void BoundInsMesh::initCubeMesh()
{
	float vertices[] = {
		-0.5, 0.5, -0.5,
		-0.5, 0.5, 0.5,
		0.5, 0.5, 0.5,
		0.5, 0.5, -0.5,
		-0.5, -0.5, -0.5,
		-0.5, -0.5, 0.5,
		0.5, -0.5, 0.5,
		0.5, -0.5, -0.5,
	};
	unsigned int elements[] = {
		0, 1,
		0, 3,
		//0, 4,
		2, 1,
		2, 3,
		2, 6,
		5, 1,
		5, 4,
		5, 6,
		7, 3,
		7, 6,
		7, 4
	};
	initCustomMesh("CubeMesh", reinterpret_cast<const unsigned char*>(vertices), sizeof(vertices), \
		reinterpret_cast< const unsigned char*>(elements), sizeof(elements));
}

void BoundInsMesh::initCustomMesh(const std::string &meshName, const unsigned char *pVertexData, int vertexNum, const unsigned char *pIndexData, int indexNum)
{
	if (mpCubeBoxVData == 0)
	{
		mpCubeBoxVData = new float[vertexNum / sizeof(float)];
		memcpy(mpCubeBoxVData, pVertexData, vertexNum);
	}

	const char *cubeMeshName = meshName.c_str();

	NameParamMap paramMap;
	IMesh *pMeshData = gEngModule->pMeshMgr->create(cubeMeshName, paramMap);
	mpMesh = pMeshData;
	pMeshData->setInstanceRender(true);

	//IMesh *pMeshData = gEngModule->pMeshMgr->getDataPtr(meshHandle);
	VertexData *pNewVertexData = pMeshData->getVertexData();
	IndexData *pNewIndexData = pMeshData->getIndexData();

	pNewVertexData->pMemData = reinterpret_cast<unsigned char*>(mpCubeBoxVData);
	pNewVertexData->elementSize = sizeof(float)* 3;
	pNewVertexData->elementCount = vertexNum / pNewVertexData->elementSize;
	pNewVertexData->type = VertexDataSortType::P3;

	if (mpCubeBoxIData == 0)
	{
		mpCubeBoxIData = new unsigned int[indexNum / sizeof(unsigned int)];
		memcpy(mpCubeBoxIData, pIndexData, indexNum);
	}

	pNewIndexData->elementSize = sizeof(unsigned int);
	pNewIndexData->elementCount = indexNum / pNewIndexData->elementSize;
	pNewIndexData->type = IndexType::BIT32;
	pNewIndexData->pMemData = reinterpret_cast<unsigned char*>(mpCubeBoxIData);

	pMeshData->updateHWData();

	StaticRenderable *pStaticRender = new StaticRenderable(gEngModule->pMeshMgr->getHandle(cubeMeshName), 0);
	IMaterial *pMat = gEngModule->pMaterialMgr->create("Res/mat_util/helper_instance.mtl");
	pStaticRender->setMaterial(gEngModule->pMaterialMgr->getHandle(pMat->getName()));
	mpRenderComp->addRenderable(pStaticRender);
	mpRenderComp->showAABB(false);
	mpRenderComp->setNeedClip(false);
}
