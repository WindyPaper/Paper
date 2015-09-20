#ifndef _IGAME_OBJ_COMPONENT_H_
#define _IGAME_OBJ_COMPONENT_H_

#include <string>
#include "util/Paper_Math.h"
#include "ResourceSystem/IMesh.h"

class IGameObject;
class Renderable;
class Node;

enum GameObjCompType
{
	RenderComp = 1 << 0,
	AnimComp = 1 << 1,
	PhysicComp = 1 << 2,
	LightComp = 1 << 3,
	ParticleComp = 1 << 4,
};

class IGameObjComponent
{
public:
	IGameObjComponent() { mpParent = 0; }
	virtual ~IGameObjComponent() {}

	virtual GameObjCompType getCompType() const = 0;

protected:
	IGameObject *mpParent;
};

class IGameObjRenderComp : public IGameObjComponent, public sigslot::has_slots<>
{
public:
	virtual ~IGameObjRenderComp() {}

	virtual GameObjCompType getCompType() const { return GameObjCompType::RenderComp; }

	virtual void setMaterial(const std::string &name) = 0;
	virtual void setMaterial(const std::string &name, const int index) = 0;

	/*virtual void setPosition(const math::Vector3f &pos) = 0;
	virtual void setPosition(const float x, const float y, const float z) = 0;
	virtual const math::Vector3f &getPostion() const = 0;

	virtual void setScale(const math::Vector3f &scale) = 0;
	virtual void setScale(const float x, const float y, const float z) = 0;
	virtual const math::Vector3f &getScale() const = 0;

	virtual void setOrientation(const math::Quaternion &quat) = 0;
	virtual void setOrientation(const float x, const float y, const float z, const float w) = 0;
	virtual const math::Quaternion &getOrientation() const = 0;*/
	virtual Node *getOwnNode() = 0;

	virtual void addRenderable(Renderable *pRenderable) = 0;
	virtual void modifyRenderable(Renderable *pRenderable, int index) = 0;
	virtual Renderable *getRenderable(int index) = 0;

	virtual void updateRenderable(MeshHandle meshHandle) {
		assert(false);
	}
};

#endif