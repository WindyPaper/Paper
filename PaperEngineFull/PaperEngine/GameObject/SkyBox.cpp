#include "util/PrecompileHead.h"
#include "GameObject/SkyBox.h"
#include "util/Global.h"
#include "GameObject/StaticRenderable.h"

SkyBox::SkyBox(const std::string &name) :
GameObject(name)
{

}

SkyBox::~SkyBox()
{

}

void SkyBox::init()
{
	static float vertices[] = {
		// front
		-1.0, -1.0, 1.0,
		1.0, -1.0, 1.0,
		1.0, 1.0, 1.0,
		-1.0, 1.0, 1.0,
		// back
		-1.0, -1.0, -1.0,
		1.0, -1.0, -1.0,
		1.0, 1.0, -1.0,
		-1.0, 1.0, -1.0,
	};

	static unsigned int elements[] = { 
		// front
		2, 1, 0,
		0, 3, 2,
		// top
		6, 5, 1,
		1, 2, 6,
		// back
		5, 6, 7,
		7, 4, 5,
		// bottom
		3, 0, 4,
		4, 7, 3,
		// left
		1, 5, 4,
		4, 0, 1,
		// right
		6, 2, 3,
		3, 7, 6,
	};

	/*static unsigned int elements[] = {
		// front
		3, 0, 1,
		3, 1, 2,
		// top
		2, 3, 7,
		6, 2, 7,
		// back
		5, 4, 7, 
		6, 5, 7,
		// bottom
		1, 0, 4,
		5, 1, 4, 
		// left
		4, 0, 3, 
		7, 4, 3,
		// right
		5, 1, 2,
		6, 5, 2
	};*/

	const char *cubeMeshName = "SkyBoxMesh";

	NameParamMap paramMap;
	IMesh *pMeshData = gEngModule->pMeshMgr->create(cubeMeshName, paramMap);
	//pMeshData->setInstanceRender(true);

	//IMesh *pMeshData = gEngModule->pMeshMgr->getDataPtr(meshHandle);
	VertexData *pNewVertexData = pMeshData->getVertexData();
	IndexData *pNewIndexData = pMeshData->getIndexData();

	pNewVertexData->pMemData = reinterpret_cast<unsigned char*>(vertices);
	pNewVertexData->elementSize = sizeof(float)* 3;
	pNewVertexData->elementCount = 8;
	pNewVertexData->type = VertexDataSortType::P3;

	pNewIndexData->elementSize = sizeof(unsigned int);
	pNewIndexData->elementCount = 36;
	pNewIndexData->type = IndexType::BIT32;
	pNewIndexData->pMemData = reinterpret_cast<unsigned char*>(elements);

	pMeshData->updateHWData();

	StaticRenderable *pStaticRender = new StaticRenderable(gEngModule->pMeshMgr->getHandle(cubeMeshName), 0);
	IMaterial *pMat = gEngModule->pMaterialMgr->create("Res/mat_util/sky_box.mtl");
	pStaticRender->setMaterial(gEngModule->pMaterialMgr->getHandle(pMat->getName()));
	mpRenderComp->addRenderable(pStaticRender);
	mpRenderComp->showAABB(false);

	//create cube map
	NameParamMap texParamMap;
	texParamMap["TextureType"] = "CubeMap";
	/*texParamMap["0"] = "Res/texture/left.png";
	texParamMap["1"] = "Res/texture/right.png";
	texParamMap["2"] = "Res/texture/top.png";
	texParamMap["3"] = "Res/texture/bottom.png";
	texParamMap["4"] = "Res/texture/back.png";
	texParamMap["5"] = "Res/texture/front.png";*/

	texParamMap["0"] = "Res/texture/left.jpg";
	texParamMap["1"] = "Res/texture/right.jpg";
	texParamMap["2"] = "Res/texture/top.jpg";
	texParamMap["3"] = "Res/texture/bottom.jpg";
	texParamMap["4"] = "Res/texture/back.jpg";
	texParamMap["5"] = "Res/texture/front.jpg";
	ITexture *pCubeMap = gEngModule->pTexMgr->create("SkyBoxCubeMap", texParamMap);
	TexHandle texHandle = gEngModule->pTexMgr->getHandle(pCubeMap->getName());
	pMat->setTexture(texHandle, 0);
}
