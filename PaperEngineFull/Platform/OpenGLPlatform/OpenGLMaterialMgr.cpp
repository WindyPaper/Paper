#include "OpenGLPlatform/RenderPreCompileHead.h"
#include "util/Log.h"
#include "OpenGLPlatform/OpenGLMaterialMgr.h"

template <> OpenGLMaterialMgr *Singleton<OpenGLMaterialMgr>::msInstance = 0;

OpenGLMaterialMgr& OpenGLMaterialMgr::getInstance(void)
{
	assert(msInstance);
	return *msInstance;
}

OpenGLMaterialMgr* OpenGLMaterialMgr::getInstancePtr(void)
{
	return msInstance;
}

OpenGLMaterialMgr::OpenGLMaterialMgr()
{
	pGlobalShaderParam = new GlobalShaderParamMng();
}

OpenGLMaterialMgr::~OpenGLMaterialMgr()
{
	for (ResHandleMap::iterator iter = mResHandleMap.begin(); iter != mResHandleMap.end();)
	{
		//deleteData(iter->second);
		MaterialHandle handle = iter->second;

		IMaterial *pMaterial = mMaterialContain.dereference(handle);
		if (pMaterial)
		{
			ResHandleMap::iterator findIter = mResHandleMap.find(pMaterial->getName());
			if (findIter != mResHandleMap.end())
			{
				iter = mResHandleMap.erase(findIter);
			}
			else
			{
				++iter;
			}
		}
		mMaterialContain.release(handle);
	}
	
	SAFE_DELETE(pGlobalShaderParam);
}

IMaterial * OpenGLMaterialMgr::create(const std::string &name)
{
	IMaterial *pMaterial = getDataPtr(name);
	if (pMaterial != 0)
	{
		return pMaterial;
	}
	else
	{
		return _createMaterialFast(name);
	}
}

IMaterial * OpenGLMaterialMgr::create(const std::string &name, const NameParamMap &paramMap)
{
	assert(0);
	return 0;
}

IMaterial * OpenGLMaterialMgr::getDataPtr(MaterialHandle &handle)
{
	return mMaterialContain.dereference(handle);
}

IMaterial * OpenGLMaterialMgr::getDataPtr(const std::string &name)
{
	ResHandleMap::iterator iter = mResHandleMap.find(name);
	if (iter != mResHandleMap.end())
	{
		return mMaterialContain.dereference(iter->second);
	}
	else
	{
		return 0;
	}
}

MaterialHandle OpenGLMaterialMgr::getHandle(const std::string &name)
{
	ResHandleMap::iterator iter = mResHandleMap.find(name);
	if (iter != mResHandleMap.end())
	{
		return iter->second;
	}

	static MaterialHandle handle;
	return handle;
}

void OpenGLMaterialMgr::deleteData(MaterialHandle &handle)
{
	IMaterial *pMaterial = mMaterialContain.dereference(handle);
	if (pMaterial)
	{
		ResHandleMap::iterator iter = mResHandleMap.find(pMaterial->getName());
		if (iter != mResHandleMap.end())
		{
			mResHandleMap.erase(iter);
		}
	}
	mMaterialContain.release(handle);
}

IMaterial * OpenGLMaterialMgr::_createMaterialFast(const std::string &name)
{
	MaterialHandle handle;
	IMaterial *pMaterial = mMaterialContain.acquire(handle);
	
	if (!pMaterial->load(name))
	{
		Log::getInstance().logMsg("load Material %s Error !", name.c_str());
	}

	mResHandleMap.insert(std::pair<std::string, MaterialHandle>(name, handle));
	return pMaterial;
}

IMaterial * OpenGLMaterialMgr::_createMaterialFast(const std::string &name, const std::string &fileName)
{
	MaterialHandle handle;
	IMaterial *pMaterial = mMaterialContain.acquire(handle);

	if (!pMaterial->load(fileName))
	{
		Log::getInstance().logMsg("load Material %s Error !", fileName.c_str());
	}

	mResHandleMap.insert(std::pair<std::string, MaterialHandle>(name, handle));
	return pMaterial;
}
