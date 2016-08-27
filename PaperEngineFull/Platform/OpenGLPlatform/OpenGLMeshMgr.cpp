#include "OpenGLPlatform/RenderPreCompileHead.h"
#include "OpenGLPlatform/OpenGLMeshMgr.h"

template <> OpenGLMeshMgr *Singleton<OpenGLMeshMgr>::msInstance = 0;

OpenGLMeshMgr *OpenGLMeshMgr::getInstancePtr()
{
	return msInstance;
}

OpenGLMeshMgr &OpenGLMeshMgr::getInstance()
{
	assert(msInstance);
	return (*msInstance);
}

OpenGLMeshMgr::OpenGLMeshMgr()
{

}

OpenGLMeshMgr::~OpenGLMeshMgr()
{
	for (ResHandleMap::iterator iter = mResHandleMap.begin(); iter != mResHandleMap.end();)
	{
		//deleteData(iter->second);
		MeshHandle handle = iter->second;
		IMesh *pMesh = mMeshContain.dereference(handle);
		if (pMesh)
		{
			ResHandleMap::iterator findIter = mResHandleMap.find(pMesh->getName());
			if (findIter != mResHandleMap.end())
			{
				iter = mResHandleMap.erase(findIter);
			}
			else
			{
				++iter;
			}
		}
		mMeshContain.release(handle);
	}
}

IMesh * OpenGLMeshMgr::create(const std::string &name)
{
	IMesh *pMesh = getDataPtr(name);
	if (pMesh != 0)
	{
		return pMesh;
	}
	else
	{
		return _createMeshFast(name);
	}
}

IMesh * OpenGLMeshMgr::create(const std::string &name, const NameParamMap &paramMap)
{
	//创建单个模型实例会在batch render的时候造成位置的错误，修改一个，全部都改了
	//if we only create a single mesh instance we will get error pos(.etc) data when in batching rendering.
	IMesh *pMesh = getDataPtr(name);
	if (pMesh != 0)
	{
		return pMesh;
	}
	else
	{
		return _createMeshFast(name, paramMap);
	}
}

IMesh * OpenGLMeshMgr::getDataPtr(MeshHandle &handle)
{
	return mMeshContain.dereference(handle);
}

IMesh * OpenGLMeshMgr::getDataPtr(const std::string &name)
{
	ResHandleMap::iterator iter = mResHandleMap.find(name);
	if (iter != mResHandleMap.end())
	{
		return mMeshContain.dereference(iter->second);
	}
	else
	{
		return 0;
	}
}

MeshHandle OpenGLMeshMgr::getHandle(const std::string &name)
{
	ResHandleMap::iterator iter = mResHandleMap.find(name);
	if (iter != mResHandleMap.end())
	{
		return iter->second;
	}

	static MeshHandle handle;
	return handle;
}

void OpenGLMeshMgr::deleteData(MeshHandle &handle)
{
	IMesh *pMesh = mMeshContain.dereference(handle);
	if (pMesh)
	{
		ResHandleMap::iterator iter = mResHandleMap.find(pMesh->getName());
		if (iter != mResHandleMap.end())
		{
			mResHandleMap.erase(iter);
		}
	}
	mMeshContain.release(handle);
}

IMesh * OpenGLMeshMgr::_createMeshFast(const std::string &name)
{
	MeshHandle handle;
	IMesh *pMesh = mMeshContain.acquire(handle);
	//pMesh->setName(name);
	pMesh->load(name);

	mResHandleMap.insert(std::pair<std::string, MeshHandle>(name, handle));
	return pMesh;
}

IMesh * OpenGLMeshMgr::_createMeshFast(const std::string &name, const NameParamMap &paramMap)
{
	MeshHandle handle;
	IMesh *pMesh = mMeshContain.acquire(handle);
	pMesh->load(name, paramMap);

	mResHandleMap.insert(std::pair<std::string, MeshHandle>(name, handle));
	return pMesh;
}
