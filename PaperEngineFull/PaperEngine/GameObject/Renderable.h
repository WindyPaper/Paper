#ifndef _RENDER_ABLE_H_
#define _RENDER_ABLE_H_

#include <string>
#include "util/RenderCommand.h"
#include "util/CollionGeo.h"
#include "ResourceSystem/IMaterial.h"
#include "ResourceSystem/IMesh.h"

class IGameObjRenderComp;

class ENGINE_DLL Renderable
{
public:
	Renderable() : mBatchRenderId(0), mBatchEnable(false) {}
	virtual ~Renderable() {}

	virtual MaterialHandle getMaterial() = 0;
	virtual MeshHandle getMeshHandle() = 0;
	virtual void setMaterial(const std::string &name) = 0;
	virtual void setMaterial(MaterialHandle materialHandle) = 0;

	virtual void setParentComp(IGameObjRenderComp *parent) = 0;
	virtual IGameObjRenderComp *getParentComp() = 0;

	virtual void generateRenderCommand(RenderCommand &renderCommand) = 0;

	virtual int getBatchRenderId() const { return mBatchRenderId; }
	virtual void setBatchRenderEnable(bool isEnable) { mBatchEnable = isEnable; assert(false); } //convert vertice to world for batching render
	virtual bool getBatchRenderEnable() const { return mBatchEnable; }

	//virtual void setWorldAABB(const AABB &aabb) = 0;
	//virtual AABB getWorldAABB() const = 0;

protected:
	int mBatchRenderId; //应该要64位的 it should be 64 bit
	bool mBatchEnable;
};

#endif