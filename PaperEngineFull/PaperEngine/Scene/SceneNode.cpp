#include "util/PrecompileHead.h"
#include "Scene/SceneNode.h"
#include "Scene/SceneMgr.h"
#include "GameObject/Entity.h"

SceneNode::SceneNode(const std::string &name)
{
	mName = name;
}

SceneNode::~SceneNode()
{
	detachAllMovable();
}

SceneNode * SceneNode::createChildSceneNode(const std::string &name)
{
	SceneNode *pChild = SceneMgr::getInstance().createSceneNode(name);
	addChild(pChild);
	return pChild;
}

MovableObject * SceneNode::getMovable(const std::string &name)
{
	MovableContain::iterator iter = mMovableContain.find(name);
	if (iter != mMovableContain.end())
	{
		return iter->second;
	}

	return 0;
}

MovableObject * SceneNode::getMovable(int index)
{
	int count = getMovableCount();
	if (index > count - 1)
	{
		return 0;
	}

	MovableContain::iterator iter = mMovableContain.begin();
	while (index--)
	{
		++iter;
	}

	return iter->second;
}

int SceneNode::getMovableCount() const
{
	return mMovableContain.size();
}

void SceneNode::attachMovable(MovableObject *pMovable)
{
	MovableContain::iterator iter = mMovableContain.find(pMovable->getName());
	if (iter != mMovableContain.end())
	{
		return;
	}
	pMovable->setParentNode(this);
	mMovableContain[pMovable->getName()] = pMovable;
}


void SceneNode::detachMovableObj(MovableObject *pMovable)
{
	MovableContain::iterator iter = mMovableContain.find(pMovable->getName());
	if (iter != mMovableContain.end())
	{
		iter->second->setParentNode(0);
		mMovableContain.erase(iter);
	}
}


void SceneNode::detachAllMovable()
{
	for (MovableContain::iterator iter = mMovableContain.begin(); iter != mMovableContain.end(); ++iter)
	{
		iter->second->setParentNode(0);
	}
	mMovableContain.clear();
}
