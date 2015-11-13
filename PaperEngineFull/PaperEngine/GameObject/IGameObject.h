#ifndef _I_GAME_OBJECT_H_
#define _I_GAME_OBJECT_H_

#include "util/Paper_Math.h"
#include "GameObject/IGameObjComponent.h"

class IGameObject
{
public:
	virtual ~IGameObject() {}

	virtual void addChild(IGameObject *ent) = 0;
	virtual void removeChild(IGameObject *ent) = 0;

	virtual void setPosition(const math::Vector3f &pos) = 0;
	virtual const math::Vector3f &getPosition() const = 0;

	virtual void setScale(const math::Vector3f &scale) = 0;
	virtual const math::Vector3f &getScale() const = 0;

	virtual void setOrientation(const math::Quaternion &quat) = 0;
	virtual void setOrientation(const float x, const float y, const float z, const float w) = 0;
	virtual const math::Quaternion &getOrientation() const = 0;

	virtual void setObjectComp(IGameObjComponent *comp) = 0;
	virtual IGameObjComponent *getComp(const GameObjCompType type) = 0;
	//virtual void setTranslate(const math::Vector3f &pos)

	virtual const std::string &getName() = 0;
};

class IGameObjSystem
{
public:
	virtual ~IGameObjSystem() {}

	virtual IGameObject *createStaticGeoObj(const std::string &gameObjName, const std::string &meshName, const math::Vector3f &pos, const math::Quaternion &quat) = 0;
	virtual IGameObject *createPlane(const int width, const int height, const int seq, const math::Vector3f &pos, const math::Quaternion &quat) = 0;
	virtual void destoryAllStaticGeoObj() = 0;
};

#endif