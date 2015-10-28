#ifndef _STATIC_RENDERABLE_H_
#define _STATIC_RENDERABLE_H_

#include "ResourceSystem/ResHandle.h"
#include "GameObject/Renderable.h"
#include "ResourceSystem/IMesh.h"


class ENGINE_DLL StaticRenderable : public Renderable
{
public:
	StaticRenderable();
	StaticRenderable(const MeshHandle &meshResHandle, const int subIndex);

	virtual MaterialHandle getMaterial();
	virtual MeshHandle getMeshHandle();
	virtual void setMaterial(const std::string &name);
	virtual void setMaterial(MaterialHandle materialHandle);

	virtual void setParentComp(IGameObjRenderComp *parent);
	virtual IGameObjRenderComp *getParentComp();
	virtual void generateRenderCommand(RenderCommand &renderCommand);

	virtual AABB getLocalAABB();

protected:
	MaterialHandle mMaterialHandle;
	IGameObjRenderComp *mpOwner;
	MeshHandle mMeshHandle;
	int mSubIndex;

	AABB mWorldAABB;
};

#endif