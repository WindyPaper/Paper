#include "util/PrecompileHead.h"
#include "util/Global.h"
#include "GameObject/StaticRenderable.h"
#include "GameObject/IGameObjComponent.h"
#include "Scene/SceneNode.h"
#include "XNAMath/xnamath.h"

StaticRenderable::StaticRenderable() :
mIsConvertVerticeDirty(true)
{

}

StaticRenderable::StaticRenderable(const MeshHandle &meshResHandle, const int subIndex) :
mIsConvertVerticeDirty(true)
{
	mMeshHandle = meshResHandle;
	mSubIndex = subIndex;
}

MaterialHandle StaticRenderable::getMaterial()
{
	return mMaterialHandle;
}

MeshHandle StaticRenderable::getMeshHandle()
{
	return mMeshHandle;
}

void StaticRenderable::setMaterial(const std::string &name)
{
	mMaterialHandle = gEngModule->pMaterialMgr->getHandle(name);
	mBatchRenderId = mMaterialHandle;
}

void StaticRenderable::setMaterial(MaterialHandle materialHandle)
{
	mMaterialHandle = materialHandle;
	mBatchRenderId = mMaterialHandle;
}

void StaticRenderable::setParentComp(IGameObjRenderComp *parent)
{
	mpOwner = parent;
}

IGameObjRenderComp * StaticRenderable::getParentComp()
{
	return mpOwner;
}

void StaticRenderable::generateRenderCommand(RenderCommand &renderCommand)
{
	IMeshMgr *pMeshMgr = gEngModule->pMeshMgr;
	IMesh *pMeshData = pMeshMgr->getDataPtr(mMeshHandle);
	
	if (pMeshData)
	{
		pMeshData->generateRenderCommand(renderCommand, mSubIndex);
	}
	else
	{
		Log::getInstance().logMsg("Entity %s lost mesh !!!", mpOwner->getName());
	}
	
}

void StaticRenderable::setBatchRenderEnable(bool isEnable)
{
	mBatchEnable = isEnable;
	if (mBatchEnable)
	{
		convertVerticeToWorld();
	}
}

AABB StaticRenderable::getLocalAABB()
{
	IMeshMgr *pMeshMgr = gEngModule->pMeshMgr;
	IMesh *pMeshData = pMeshMgr->getDataPtr(mMeshHandle);
	return pMeshData->getAABB();
}

void StaticRenderable::convertVerticeToWorld()
{
	if (mIsConvertVerticeDirty == false)
	{
		return;
	}
	mIsConvertVerticeDirty = false;

	IMeshMgr *pMeshMgr = gEngModule->pMeshMgr;
	IMesh *pMeshData = pMeshMgr->getDataPtr(mMeshHandle);

	VertexDataSortType sortType = pMeshData->getVertexData()->type;
	VertexData *pVertexData = pMeshData->getVertexData();
	unsigned char *pData = pVertexData->pMemData;
	unsigned char *pCurrData = pData;

	XMMATRIX transformMatrix = mpOwner->getOwnNode()->getAllTransformMatrix().m;

	switch (sortType)
	{
	case P3F2F3:
	{
		int offset = sizeof(float)* 8;
		XMVECTOR pos;

		for (int i = 0; i < pVertexData->elementCount; ++i)
		{
			pos.x = *(float*)pCurrData;
			pos.y = *(float*)(pCurrData + sizeof(float));
			pos.z = *(float*)(pCurrData + sizeof(float)* 2);
			pos = XMVector3Transform(pos, transformMatrix);
			memcpy(pCurrData, &pos.x, sizeof(float)* 3);
			pCurrData += offset;
		}
		pMeshData->bindVertexData();
	}
		break;
	case P3UVF2:
	{
		int offset = sizeof(float)* 5;
		XMVECTOR pos;

		for (int i = 0; i < pVertexData->elementCount; ++i)
		{
			pos.x = *(float*)pCurrData;
			pos.y = *(float*)(pCurrData + sizeof(float));
			pos.z = *(float*)(pCurrData + sizeof(float)* 2);
			pos = XMVector3Transform(pos, transformMatrix);
			memcpy(pCurrData, &pos.x, sizeof(float)* 3);
			pCurrData += offset;
		}
		pMeshData->bindVertexData();
	}
		break;
	case P3I1F2:
		break;
	case P3:
		break;
	case P3INS:
		break;
	case UNKNOW:
		break;
	default:
		break;
	}
}

