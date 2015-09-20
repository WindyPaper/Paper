#include "Precompiled.h"
#include <assert.h>
#include "UIRenderable.h"
#include "util/Global.h"

UIRenderable::UIRenderable() :
mVertexCount(0)
{
	static int i = 0;

	static char buffer[128];
	memset(buffer, 0, 128);
	sprintf_s(buffer, "MyGuiMesh%d", i);
	++i;

	mResName = buffer;
}

UIRenderable::~UIRenderable()
{

}

MaterialHandle UIRenderable::getMaterial()
{
	return mMatHandle;
}

void UIRenderable::setMaterial(const std::string &name)
{
	assert(false);
}

void UIRenderable::setMaterial(MaterialHandle materialHandle)
{
	mMatHandle = materialHandle;
}

void UIRenderable::generateRenderCommand(RenderCommand &renderCommand)
{
	gEngModule->pMeshMgr->getDataPtr(mMeshHandle)->generateRenderCommand(renderCommand);
}

void UIRenderable::setUIVertexData(unsigned char *pData, int count)
{
	IMesh *pMesh = 0;
	if (mMeshHandle.isNull() == false)
	{
		//mMatHandle = gEngModule->pMeshMgr->
		pMesh = gEngModule->pMeshMgr->create(mResName);
		mMeshHandle = gEngModule->pMeshMgr->getHandle(mResName);
	}
	else
	{
		pMesh = gEngModule->pMeshMgr->getDataPtr(mMeshHandle);
	}

	pMesh->setMeshMemVertexData(pData, count, VertexDataSortType::P3I1F2);
	//setVertexCount(count);

	//Éú³ÉUIË÷Òý
	//generate ui index data
	unsigned int *pIndexData = reinterpret_cast<unsigned int *>(pData + sizeof(Vertex_P3I1F2)* count);
	unsigned int *pCurrIndexData = pIndexData;
	for (int i = 0; i < count; ++i)
	{
		*pCurrIndexData = i;
		++pCurrIndexData;
	}
	setUIIndexData(pIndexData, count);
	//delete[] pIndexData;
}

void UIRenderable::setUIIndexData(unsigned int *pData, int count)
{
	assert(!mMeshHandle.isNull());

	IMesh *pMesh = gEngModule->pMeshMgr->getDataPtr(mMeshHandle);
	pMesh->setMeshMemIndexData(reinterpret_cast<unsigned char*>(pData), count, BIT32);
}

void UIRenderable::setVertexCount(size_t _value)
{
	if (mVertexCount != _value)
	{
		IMesh *pMesh = 0;
		if (mMeshHandle.isNull() == false)
		{
			gEngModule->pMeshMgr->deleteData(mMeshHandle);
		}

		//mMatHandle = gEngModule->pMeshMgr->
		NameParamMap paramMap;
		pMesh = gEngModule->pMeshMgr->create(mResName, paramMap);
		mMeshHandle = gEngModule->pMeshMgr->getHandle(mResName);

		int renderMeshByteSize = (sizeof(Vertex_P3I1F2)+sizeof(uint)) * _value;

		//vertex vertex vertex index index index
		unsigned char *pVertexData = new unsigned char[renderMeshByteSize];
		memset(pVertexData, 0, renderMeshByteSize);

		pMesh = gEngModule->pMeshMgr->getDataPtr(mMeshHandle);
		pMesh->setMeshMemData(reinterpret_cast<unsigned char *>(pVertexData));
		setUIVertexData(pVertexData, _value);

		mVertexCount = _value;
	}
	
}

size_t UIRenderable::getVertexCount()
{
	return mVertexCount;
}

MyGUI::Vertex* UIRenderable::lock()
{
	IMesh *pMesh = gEngModule->pMeshMgr->getDataPtr(mMeshHandle);
	return reinterpret_cast<MyGUI::Vertex*>(pMesh->getVertexData()->pMemData);
}

void UIRenderable::unlock()
{
	IMesh *pMesh = gEngModule->pMeshMgr->getDataPtr(mMeshHandle);
	
	pMesh->updateHWData();
}

void UIRenderable::setRenderIndexValue(int value)
{
	IMesh *pMesh = gEngModule->pMeshMgr->getDataPtr(mMeshHandle);

	pMesh->setRenderIndexValue(value);
}

MeshHandle UIRenderable::getMeshHandle()
{
	return mMeshHandle;
}
