#ifndef _GAME_OBJECT_H_
#define _GAME_OBJECT_H_

#include "GameObject/IGameObject.h"

class ENGINE_DLL GameObject : public IGameObject
{
public:
	GameObject();
	GameObject(const std::string &name);
	virtual ~GameObject();

	virtual void addChild(IGameObject *ent);
	virtual void removeChild(IGameObject *ent);

	virtual void setPosition(const math::Vector3f &pos);
	virtual const math::Vector3f &getPosition() const;

	virtual void setScale(const math::Vector3f &scale);
	virtual const math::Vector3f &getScale() const;

	virtual void setOrientation(const math::Quaternion &quat);
	virtual void setOrientation(const float x, const float y, const float z, const float w);
	virtual const math::Quaternion &getOrientation() const;

	virtual void setObjectComp(IGameObjComponent *comp);
	virtual IGameObjComponent *getComp(const GameObjCompType type);

protected:
	IGameObjRenderComp *mpRenderComp;

	std::string mName;
};

#endif