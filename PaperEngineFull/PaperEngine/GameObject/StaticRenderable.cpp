#include "util/PrecompileHead.h"
#include "util/Global.h"
#include "GameObject/StaticRenderable.h"
#include "GameObject/IGameObjComponent.h"

StaticRenderable::StaticRenderable()
{

}

StaticRenderable::StaticRenderable(const MeshHandle &meshResHandle, const int subIndex)
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
}

void StaticRenderable::setMaterial(MaterialHandle materialHandle)
{
	mMaterialHandle = materialHandle;
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

AABB StaticRenderable::getLocalAABB()
{
	IMeshMgr *pMeshMgr = gEngModule->pMeshMgr;
	IMesh *pMeshData = pMeshMgr->getDataPtr(mMeshHandle);
	return pMeshData->getAABB();
}
