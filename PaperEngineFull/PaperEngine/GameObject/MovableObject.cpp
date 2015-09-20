#include "util/PrecompileHead.h"
#include "GameObject/MovableObject.h"
#include "Scene/Node.h"

MovableObject::MovableObject()
{
	mVisible = true;
	mpParentNode = 0;
}

MovableObject::MovableObject(const std::string name)
{
	mVisible = true;
	mpParentNode = 0;
	mName = name;
}

MovableObject::~MovableObject()
{
	if (mpParentNode)
	{
		
	}
}
