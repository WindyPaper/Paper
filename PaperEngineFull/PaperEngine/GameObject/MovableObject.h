#ifndef _MOVABLE_OBJECT_H_
#define _MOVABLE_OBJECT_H_

#include <string>
#include "util/Platform.h"

class Node;
class IRenderSequence;

class ENGINE_DLL MovableObject
{
public:
	MovableObject();
	MovableObject(const std::string name);
	virtual ~MovableObject();

	void setParentNode(Node *pParentNode) { mpParentNode = pParentNode; }
	Node *getParentNode() { return mpParentNode; }
	const Node *getParentNode() const { return mpParentNode; }

	void setVisible(bool isVisible) { mVisible = isVisible; }
	bool isVisible() const { return mVisible; }

	const std::string &getName() const { return mName; }
	void setName(const std::string &name) { mName = name; }

	virtual void pushToRenderSequence(IRenderSequence *pRenderSequence) = 0;

protected:
	Node *mpParentNode;
	bool mVisible;
	std::string mName;

};

#endif