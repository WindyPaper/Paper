#ifndef _NODE_H_
#define _NODE_H_

#include <string>
#include <map>
#include <vector>
#include "util/Paper_Math.h"
#include "util/Paper_Quat.h"

class Node
{
public:
	typedef std::map<std::string, Node*> ChildMapContain;
	typedef std::vector<Node*> ChildVectorContain;

	Node();
	Node(const std::string &name);
	virtual ~Node();

	Node *getChild(const std::string &name);
	Node *getChild(int index);
	int getChildCount() const;

	virtual void addChild(Node *pNode);
	virtual Node* removeChild(Node *pNode);
	virtual void removeAllChildren();

	virtual void setPosition(const math::Vector3f &pos);
	virtual const math::Vector3f &getPosition() const;
	virtual const math::Vector3f &getWorldPosition() const;

	virtual void setScale(const math::Vector3f &scale);
	virtual void setScale(const float x, const float y, const float z);
	virtual const math::Vector3f &getScale() const;
	virtual const math::Vector3f &getWorldScale() const;

	virtual void setOrientation(const math::Quaternion &quat);
	virtual void setOrientation(const float x, const float y, const float z, const float w);
	virtual const math::Quaternion &getOrientation() const;
	virtual const math::Quaternion &getWorldOrientation() const;

	virtual void setParent(Node *parent);
	virtual Node *getParent();

	virtual const std::string &getName() const;

	const math::Matrix44 &getAllTransformMatrix();

	virtual void update();

protected:
	virtual void updateFromParent();

protected:
	Node *mpParent;
	std::string mName;
	
	ChildMapContain mChildMapContain;
	ChildMapContain::iterator mChildMapContainIter;
	ChildMapContain::const_iterator mConstChildMapContainIter;

	ChildVectorContain mChildVecContain;
	ChildVectorContain::iterator mChildVecContainIter;

	bool mIsInheritOrient;
	bool mIsInheritScale;

	math::Quaternion mLocalOrient;
	math::Vector3f mLocalPos;
	math::Vector3f mLocalScale;

	math::Quaternion mWorldOrient;
	math::Vector3f mWorldPos;
	math::Vector3f mWorldScale;
};

#endif