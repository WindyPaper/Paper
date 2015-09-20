#include "util/PrecompileHead.h"
#include "GameObject/GameObject.h"
#include "Scene/Node.h"

GameObject::GameObject()
{

}

GameObject::GameObject(const std::string &name)
{
	mName = name;
}

GameObject::~GameObject()
{

}

void GameObject::addChild(IGameObject *ent)
{

}

void GameObject::removeChild(IGameObject *ent)
{

}

void GameObject::setPosition(const math::Vector3f &pos)
{
	if (mpRenderComp)
		mpRenderComp->getOwnNode()->setPosition(pos);
}

const math::Vector3f & GameObject::getPosition() const
{
	if (mpRenderComp)
		return mpRenderComp->getOwnNode()->getPosition();
}

void GameObject::setScale(const math::Vector3f &scale)
{
	if (mpRenderComp)
		mpRenderComp->getOwnNode()->setScale(scale);
}

const math::Vector3f & GameObject::getScale() const
{
	if (mpRenderComp)
		return mpRenderComp->getOwnNode()->getScale();
	
	static math::Vector3f zero;
	return zero;
}

void GameObject::setOrientation(const math::Quaternion &quat)
{
	if (mpRenderComp)
		mpRenderComp->getOwnNode()->setOrientation(quat);
}

void GameObject::setOrientation(const float x, const float y, const float z, const float w)
{
	if (mpRenderComp)
		mpRenderComp->getOwnNode()->setOrientation(x, y, z, w);
}

const math::Quaternion & GameObject::getOrientation() const
{
	if (mpRenderComp)
		return mpRenderComp->getOwnNode()->getOrientation();

	static math::Quaternion iquat;
	return iquat;
}

void GameObject::setObjectComp(IGameObjComponent *comp)
{
	if (comp->getCompType() == GameObjCompType::RenderComp)
	{
		mpRenderComp = dynamic_cast<IGameObjRenderComp*>(comp);
	}
}

IGameObjComponent * GameObject::getComp(const GameObjCompType type)
{
	if (type == GameObjCompType::RenderComp)
	{
		return mpRenderComp;
	}

	return 0;
}
