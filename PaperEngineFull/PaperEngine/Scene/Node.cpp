#include "util/PrecompileHead.h"
#include "Scene/Node.h"
#include "util/Log.h"

Node::Node() :
mpParent(0)
{
	mLocalPos = math::Vector3f(0.0, 0.0, 0.0);
	mLocalScale = math::Vector3f(1.0, 1.0, 1.0);
	mLocalOrient = math::Quaternion(0.0, 0.0, 0.0, 1.0);

	mWorldPos = math::Vector3f(0.0, 0.0, 0.0);
	mWorldScale = math::Vector3f(1.0, 1.0, 1.0);
	mWorldOrient = math::Quaternion(0.0, 0.0, 0.0, 1.0);

	mIsInheritOrient = true;
	mIsInheritScale = true;

	mName = "Default Node";
}

Node::Node(const std::string &name) :
mName(name),
mpParent(0)
{
	mLocalPos = math::Vector3f(0.0, 0.0, 0.0);
	mLocalScale = math::Vector3f(1.0, 1.0, 1.0);
	mLocalOrient = math::Quaternion(0.0, 0.0, 0.0, 1.0);

	mWorldPos = math::Vector3f(0.0, 0.0, 0.0);
	mWorldScale = math::Vector3f(1.0, 1.0, 1.0);
	mWorldOrient = math::Quaternion(0.0, 0.0, 0.0, 1.0);

	mIsInheritOrient = true;
	mIsInheritScale = true;
}

Node::~Node()
{
	removeAllChildren();

	if (mpParent)
	{
		mpParent->removeChild(this);
		mpParent = 0;
	}
	
}

Node * Node::getChild(const std::string &name)
{
	mChildMapContainIter = mChildMapContain.find(name);
	if (mChildMapContainIter != mChildMapContain.end())
	{
		return mChildMapContainIter->second;
	}

	return 0;
}

Node * Node::getChild(int index)
{
	int count = getChildCount();
	if (index > count - 1)
	{
		return 0;
	}

	return mChildVecContain[index];
}

int Node::getChildCount() const
{
	return mChildVecContain.size();
}

void Node::addChild(Node *pNode)
{
	pNode->setParent(this);
	mChildMapContain.insert(std::pair<std::string, Node*>(pNode->getName(), pNode));
	mChildVecContain.push_back(pNode);
}

Node* Node::removeChild(Node *pNode)
{
	pNode->setParent(0);

	mChildMapContainIter = mChildMapContain.find(pNode->getName());
	if (mChildMapContainIter != mChildMapContain.end())
	{
		mChildMapContainIter->second->setParent(0);
		mChildMapContain.erase(mChildMapContainIter);
	}

	for (mChildVecContainIter = mChildVecContain.begin(); mChildVecContainIter != mChildVecContain.end();)
	{
		if (*mChildVecContainIter == pNode)
		{
			mChildVecContainIter = mChildVecContain.erase(mChildVecContainIter);
			break;
		}
		++mChildVecContainIter;
	}
	return pNode;
}

void Node::removeAllChildren()
{
	for (mChildMapContainIter = mChildMapContain.begin(); mChildMapContainIter != mChildMapContain.end(); ++mChildMapContainIter)
	{
		mChildMapContainIter->second->setParent(0);
	}
	mChildMapContain.clear();
	mChildVecContain.clear();
}

void Node::setPosition(const math::Vector3f &pos)
{
	mLocalPos = pos;
}

const math::Vector3f & Node::getPosition() const
{
	return mLocalPos;
}

const math::Vector3f & Node::getWorldPosition() const
{
	return mWorldPos;
}

void Node::setScale(const math::Vector3f &scale)
{
	mLocalScale = scale;
}

void Node::setScale(const float x, const float y, const float z)
{
	mLocalScale.x = x;
	mLocalScale.y = y;
	mLocalScale.z = z;
}

const math::Vector3f & Node::getScale() const
{
	return mLocalScale;
}

const math::Vector3f & Node::getWorldScale() const
{
	return mWorldScale;
}

void Node::setOrientation(const math::Quaternion &quat)
{
	mLocalOrient = quat;
}

void Node::setOrientation(const float x, const float y, const float z, const float w)
{
	mLocalOrient.vec4.x = x;
	mLocalOrient.vec4.y = y;
	mLocalOrient.vec4.z = z;
	mLocalOrient.vec4.w = w;
}

const math::Quaternion & Node::getOrientation() const
{
	return mLocalOrient;
}

const math::Quaternion & Node::getWorldOrientation() const
{
	return mWorldOrient;
}

void Node::setParent(Node *parent)
{
	if (mpParent != 0 && parent != 0)
	{
		Log::getInstance().logMsg("Node has parent!!!");
		return;
	}
	mpParent = parent;
}

Node * Node::getParent()
{
	return mpParent;
}

const std::string & Node::getName() const
{
	return mName;
}

const math::Matrix44 Node::getAllTransformMatrix()
{
	math::Matrix44 matrix;
	matrix.initFullTransform(getWorldPosition(), getWorldScale(), getWorldOrientation());
	return matrix;
}

void Node::update()
{
	updateFromParent();
	for (mChildMapContainIter = mChildMapContain.begin(); mChildMapContainIter != mChildMapContain.end(); ++mChildMapContainIter)
	{
		mChildMapContainIter->second->update();
	}
}

void Node::updateFromParent()
{

	if (mpParent)
	{
		const math::Quaternion &parentQuat = mpParent->getOrientation();
		if (mIsInheritOrient)
		{
			mWorldOrient = parentQuat * mLocalOrient;
		}
		else
		{
			mWorldOrient = mLocalOrient;
		}

		const math::Vector3f &scale = mpParent->getScale();
		if (mIsInheritScale)
		{
			mWorldScale = scale.mul(scale);
		}
		else
		{
			mWorldScale = mLocalScale;
		}

		math::Vector3f scalePos = mLocalPos.mul(scale);
		mWorldPos = scalePos.rotate(parentQuat);
		mWorldPos += mpParent->getWorldPosition();
	}
	else
	{
		mWorldOrient = mLocalOrient;
		mWorldScale = mLocalScale;
		mWorldPos = mLocalPos;
	}
	

}
