#ifndef _ENTITY_H_
#define _ENTITY_H_

#include <vector>
#include "ResourceSystem/ResHandle.h"
#include "ResourceSystem/IMaterial.h"
#include "ResourceSystem/IMesh.h"
#include "GameObject/IGameObjComponent.h"
#include "GameObject/MovableObject.h"
#include "util/sigslot.h"

class Renderable;
class IRenderSequence;

class ENGINE_DLL Entity : public MovableObject, public IGameObjRenderComp
{
public:
	Entity();
	Entity(const std::string &name);
	virtual ~Entity();

	virtual void setMaterial(const std::string &name);
	virtual void setMaterial(const std::string &name, const int index);

	/*virtual void setPosition(const math::Vector3f &pos);
	virtual void setPosition(const float x, const float y, const float z);
	virtual const math::Vector3f &getPostion() const;

	virtual void setScale(const math::Vector3f &scale);
	virtual void setScale(const float x, const float y, const float z);
	virtual const math::Vector3f &getScale() const;

	virtual void setOrientation(const math::Quaternion &quat);
	virtual void setOrientation(const float x, const float y, const float z, const float w);
	virtual const math::Quaternion &getOrientation() const;*/

	virtual Node *getOwnNode();

	void addChildEntity(Entity *pChildEntity);
	void removeChildEntity(Entity *pChildEntity);

	virtual void modifyRenderable(Renderable *pRenderable, int index);
	virtual void addRenderable(Renderable *pRenderable);
	Renderable *getRenderable(int index);

	virtual void pushToRenderSequence(IRenderSequence *pRenderSequence);
	
	void clearAllRenderable();

	virtual void updateRenderable(MeshHandle meshHandle);
	virtual AABB getWorldAABB();

private:
	typedef std::vector<Renderable*> SubRenderableVec;
	SubRenderableVec mSubRenderableVec;

	typedef std::vector<Entity*> SubEntityVec;
	SubEntityVec mSubEntityVec;

	Entity *mpParentEntity;

	MaterialHandle mMaterialHandle;
	std::string mName;
};

#endif