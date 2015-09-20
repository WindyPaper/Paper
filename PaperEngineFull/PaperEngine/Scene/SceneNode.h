#ifndef _SCENE_NODE_H_
#define _SCENE_NODE_H_

#include <map>
#include <string>
#include "Scene/Node.h"
#include "GameObject/MovableObject.h"

class Entity;

class SceneNode : public Node
{
public:
	SceneNode(const std::string &name);
	~SceneNode();

	SceneNode *createChildSceneNode(const std::string &name);

	MovableObject *getMovable(const std::string &name);
	MovableObject *getMovable(int index);
	int getMovableCount() const;

	void attachMovable(MovableObject *pMovable);
	void detachMovableObj(MovableObject *pMovable);
	void detachAllMovable();

private:
	typedef std::map<std::string, MovableObject*> MovableContain;
	MovableContain mMovableContain;
};

#endif