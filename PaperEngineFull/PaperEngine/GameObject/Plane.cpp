#include "util/PrecompileHead.h"
#include "GameObject/Plane.h"
#include "util/Global.h"
#include "GameObject/Renderable.h"
#include "GameObject/StaticRenderable.h"

Plane::Plane(const int width, const int height, const int seg) :
mpVerMemData(0),
mpIndexMemData(0)
{
	//fix me. generate a name
	mName = "default plane";
	
	setPlaneSize(width, height, seg);
}


Plane::Plane(const std::string &name) :
mWidth(0),
mHeight(0),
mSeg(0),
mpVerMemData(0),
mpIndexMemData(0)
{
	mName = name;
}

Plane::~Plane()
{
	SAFE_DELETE_ARRAY(mpVerMemData); 
	SAFE_DELETE_ARRAY(mpIndexMemData);
}

void Plane::setPlaneSize(const int width, const int height, const int seg)
{
	mWidth = width;
	mHeight = height;
	mSeg = seg;

	//Renderable *pRenderable = mpRenderComp->getRenderable(0);
	//MeshHandle meshHandle = pRenderable->getMeshHandle();

	const char *planename = "PlaneMesh";
	NameParamMap paramMap;
	IMesh *pMeshData = gEngModule->pMeshMgr->create(planename, paramMap);

	//IMesh *pMeshData = gEngModule->pMeshMgr->getDataPtr(meshHandle);
	VertexData *pVertexData = pMeshData->getVertexData();
	IndexData *pIndexData = pMeshData->getIndexData();

	int vertexCount = width * height;
	Vertex_P3UVF2 *pVData = new Vertex_P3UVF2[vertexCount];
	mpVerMemData = pVData;

	Vertex_P3UVF2 *pCurrVdata = pVData;
	int planeDistWidth = (width - 1) * seg;
	int planeDistHeight = (height - 1) * seg;
	for (int j = 0; j < height; ++j)
	{
		for (int i = 0; i < width; ++i)
		{	
			pCurrVdata->pos = math::Vector3f(-planeDistWidth / 2 + i * seg, 0, -planeDistHeight / 2 + j * seg);
			pCurrVdata->texcood.x = i * seg / planeDistWidth;
			pCurrVdata->texcood.y = 1.0 - j * seg / planeDistHeight;
			++pCurrVdata;
		}
	}
	pVertexData->elementCount = vertexCount;
	pVertexData->elementSize = sizeof(Vertex_P3UVF2);
	pVertexData->pMemData = reinterpret_cast<unsigned char *>(pVData);
	pVertexData->type = VertexDataSortType::P3UVF2;

	//index
	int indexCount = (width - 1) * (height - 1) * 3 * 2;
	unsigned int *pindex = new unsigned int[indexCount];
	mpIndexMemData = pindex;
	unsigned int *pCurrIndexData = pindex;
	memset(pindex, 0, indexCount);
	for (int j = 0; j < height - 1; ++j)
	{
		for (int i = 0; i < width - 1; ++i)
		{
			*pCurrIndexData = i + j * width;
			++pCurrIndexData;
			*pCurrIndexData = i + (j + 1) * width;
			++pCurrIndexData;
			*pCurrIndexData = i + (j + 1) * width + 1;
			++pCurrIndexData;

			*pCurrIndexData = i + j * width;
			++pCurrIndexData;
			*pCurrIndexData = i + (j + 1) * width + 1;
			++pCurrIndexData;
			*pCurrIndexData = i + j * width + 1;
			++pCurrIndexData;
		}
	}
	pIndexData->elementCount = indexCount;
	pIndexData->elementSize = sizeof(IndexData);
	pIndexData->type = IndexType::BIT32;
	pIndexData->pMemData = reinterpret_cast<unsigned char *>(pindex);

	pMeshData->updateHWData();
	
	//mpRenderComp->updateRenderable(gEngModule->pMeshMgr->getHandle(planename), pMeshData->getSubMeshVec());
	StaticRenderable *pStaticRender = new StaticRenderable(gEngModule->pMeshMgr->getHandle(planename), 0);
	IMaterial *pMat = gEngModule->pMaterialMgr->create("Res/mat_util/model_common.mtl");
	pStaticRender->setMaterial(gEngModule->pMaterialMgr->getHandle(pMat->getName()));
	mpRenderComp->addRenderable(pStaticRender);
}
