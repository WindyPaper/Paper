#ifndef _RENDER_ABLE_H_
#define _RENDER_ABLE_H_

#include <string>
#include "util/RenderCommand.h"
#include "ResourceSystem/IMaterial.h"

class IGameObjRenderComp;

class ENGINE_DLL Renderable
{
public:
	virtual ~Renderable() {}

	virtual MaterialHandle getMaterial() = 0;
	virtual MeshHandle getMeshHandle() = 0;
	virtual void setMaterial(const std::string &name) = 0;
	virtual void setMaterial(MaterialHandle materialHandle) = 0;

	virtual void setParentComp(IGameObjRenderComp *parent) = 0;
	virtual IGameObjRenderComp *getParentComp() = 0;

	virtual void generateRenderCommand(RenderCommand &renderCommand) = 0;
};

#endif