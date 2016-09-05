#include "util/PrecompileHead.h"
#include "util/Engine_Define.h"
#include "util/Global.h"
#include "GameObject/Entity.h"
#include "GameObject/Renderable.h"
#include "util/IRenderSequence.h"
#include "Scene/Node.h"
#include "Scene/SceneNode.h"
#include "util/Log.h"
#include "GameObject/HelperObjMgr.h"
#include "ResourceSystem/IResMgr.h"
#include "ResourceSystem/IMaterial.h"
#include "GameObject/StaticRenderable.h"

Entity::Entity() :
mpParentEntity(0),
mIsShowAABB(false),
mIsNeedClip(true)
{

}

Entity::Entity(const std::string &name) :
mpParentEntity(0),
mIsShowAABB(false),
mIsNeedClip(true)
{
	mName = name;
}

Entity::~Entity()
{
	mpParentEntity = 0;

	clearAllRenderable();
}

void Entity::setMaterial(const std::string &name)
{
	//setMaterial(name, 0);
	IMaterialMgr *pMatMgr = gEngModule->pMaterialMgr;
	MaterialHandle handle = pMatMgr->getHandle(name);
	IMaterial *pMat = 0;
	if (handle.isNull() == true)
	{
		IMaterial *pCreateMat = pMatMgr->create(name);
		if (pCreateMat != 0)
		{
			mMaterialHandle = pMatMgr->getHandle(name);
			pMat = pCreateMat;
		}
	}

	for (int i = 0; i < mSubRenderableVec.size(); ++i)
	{
		if (i == 0)
		{
			mSubRenderableVec[i]->setMaterial(mMaterialHandle);
		}
		else
		{
			SubMaterialVec subMaterialVec = pMat->getSubMaterialVec();
			IMaterial *pSubMat = subMaterialVec[i - 1];
			mSubRenderableVec[i]->setMaterial(pSubMat->getName());
		}
	}
}

void Entity::setMaterial(const std::string &name, const int index)
{
	if (index >= mSubRenderableVec.size())
	{
		Log::getInstance().logMsg("Material index error !");
		return;
	}

	IMaterialMgr *pMatMgr = gEngModule->pMaterialMgr;
	MaterialHandle handle = pMatMgr->getHandle(name);
	if (handle.isNull() == true)
	{
		IMaterial *pCreateMat = pMatMgr->create(name);
		if (pCreateMat != 0)
		{
			mMaterialHandle = pMatMgr->getHandle(name);
		}
	}
	mSubRenderableVec[index]->setMaterial(name);
}

Node * Entity::getOwnNode()
{
	return mpParentNode;
}

void Entity::addChildEntity(Entity *pChildEntity)
{
	if (pChildEntity->getOwnNode())
	{
		pChildEntity->getOwnNode()->getParent()->removeChild(pChildEntity->getOwnNode());
		mpParentNode->addChild(pChildEntity->getOwnNode());
	}
	else
	{
		SceneNode *pNewChildNode = dynamic_cast<SceneNode*>(mpParentNode)->createChildSceneNode(pChildEntity->getName());
		pChildEntity->setParentNode(pNewChildNode);
	}
	mSubEntityVec.push_back(pChildEntity);
}

void Entity::removeChildEntity(Entity *pChildEntity)
{
	if (pChildEntity->getOwnNode()->getParent() == mpParentNode)
	{
		mpParentNode->removeChild(pChildEntity->getOwnNode());
	}
}

/*
void Entity::setPosition(const math::Vector3f &pos)
{

}

void Entity::setPosition(const float x, const float y, const float z)
{

}

const math::Vector3f & Entity::getPostion() const
{
	return mpParentNode->getPosition();
}

void Entity::setScale(const math::Vector3f &scale)
{

}

void Entity::setScale(const float x, const float y, const float z)
{

}

const math::Vector3f & Entity::getScale() const
{
	return math::Vector3f(1, 1, 1);
}

void Entity::setOrientation(const math::Quaternion &quat)
{

}

void Entity::setOrientation(const float x, const float y, const float z, const float w)
{

}

const math::Quaternion & Entity::getOrientation() const
{
	return math::Quaternion();
}*/

void Entity::modifyRenderable(Renderable *pRenderable, int index)
{
	if (index < mSubRenderableVec.size())
	{
		//这种设计不合理，应该是谁创建 谁维护 后面看看有什么办法修正
		//This design of software is not correct, a correct design should be maintained by creator. 
		if (pRenderable != mSubRenderableVec[index])
		{
			SAFE_DELETE(mSubRenderableVec[index]);
			mSubRenderableVec[index] = pRenderable;
		}
	}
}

void Entity::addRenderable(Renderable *pRenderable)
{
	pRenderable->setParentComp(this);
	mSubRenderableVec.push_back(pRenderable);
}

Renderable * Entity::getRenderable(int index)
{
	if (index < mSubRenderableVec.size())
	{
		return mSubRenderableVec[index];
	}
	return 0;
}

void Entity::pushToRenderSequence(IRenderSequence *pRenderSequence)
{	
	//暂时没加入LOD
	//LOD is not add

	for (int i = 0; i < mSubRenderableVec.size(); ++i)
	{
		pRenderSequence->pushToRenderSequence(mSubRenderableVec[i]);
	}
	

	for (int i = 0; i < mSubEntityVec.size(); ++i)
	{
		Entity *pEntity = mSubEntityVec[i];
		pEntity->pushToRenderSequence(pRenderSequence);
	}

	//add aabb render
	if (mIsShowAABB)
	{
		HelperObjMgr::getInstance().addAABBBox(getWorldAABB());
	}
}

void Entity::clearAllRenderable()
{
	for (int i = 0; i < mSubRenderableVec.size(); ++i)
	{
		SAFE_DELETE(mSubRenderableVec[i]);
	}
	mSubRenderableVec.clear();
}

void Entity::updateRenderable(MeshHandle meshHandle)
{
	clearAllRenderable();
	IMesh *pMesh = gEngModule->pMeshMgr->getDataPtr(meshHandle);
	const SubMeshVec &subMeshVec = pMesh->getSubMeshVec();
	for (int i = 0; i < subMeshVec.size(); ++i)
	{
		Renderable *pRenderable = new StaticRenderable(meshHandle, i);
		addRenderable(pRenderable);
		std::string mtlName = subMeshVec[i].mtlName;
		IMaterial *pMaterial = gEngModule->pMaterialMgr->create("res/mesh/" + mtlName);
		pRenderable->setMaterial(gEngModule->pMaterialMgr->getHandle(pMaterial->getName()));
	}
}

AABB Entity::getWorldAABB()
{
	static AABB fullAABB;
	fullAABB.reset();

	for (int i = 0; i < mSubRenderableVec.size(); ++i)
	{
		StaticRenderable *p = dynamic_cast<StaticRenderable*>(mSubRenderableVec[i]);
		fullAABB.merge(p->getLocalAABB());
	}

	convertAABBToWorld(fullAABB, mpParentNode->getAllTransformMatrix());
	return fullAABB;
}
