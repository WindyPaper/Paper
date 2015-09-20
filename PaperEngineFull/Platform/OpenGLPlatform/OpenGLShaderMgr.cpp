#include "OpenGLPlatform/RenderPreCompileHead.h"
#include "OpenGLPlatform/OpenGLShaderMgr.h"

template <> OpenGLShaderMgr *Singleton<OpenGLShaderMgr>::msInstance = 0;

OpenGLShaderMgr& OpenGLShaderMgr::getInstance(void)
{
	assert(msInstance);
	return *msInstance;
}

OpenGLShaderMgr* OpenGLShaderMgr::getInstancePtr(void)
{
	return msInstance;
}

OpenGLShaderMgr::OpenGLShaderMgr()
{

}

OpenGLShaderMgr::~OpenGLShaderMgr()
{
	for (ResHandleMap::iterator iter = mResHandleMap.begin(); iter != mResHandleMap.end();)
	{
		//deleteData(iter->second);
		ShaderHandle handle = iter->second;
		IShader *pShader = mShaderContain.dereference(handle);
		if (pShader)
		{
			ResHandleMap::iterator findIter = mResHandleMap.find(pShader->getName());
			if (findIter != mResHandleMap.end())
			{
				iter = mResHandleMap.erase(findIter);
			}
			else
			{
				++iter;
			}
		}
		mShaderContain.release(handle);
	}
}

IShader * OpenGLShaderMgr::create(const std::string &name)
{
	IShader *pShader = getDataPtr(name);
	if (pShader != 0)
	{
		return pShader;
	}
	else
	{
		return _createShaderFast(name);
	}
}

IShader * OpenGLShaderMgr::create(const std::string &name, const NameParamMap &paramMap)
{
	assert(0);
	return 0;
}

IShader * OpenGLShaderMgr::getDataPtr(ShaderHandle &handle)
{
	return mShaderContain.dereference(handle);
}

IShader * OpenGLShaderMgr::getDataPtr(const std::string &name)
{
	ResHandleMap::iterator iter = mResHandleMap.find(name);
	if (iter != mResHandleMap.end())
	{
		return mShaderContain.dereference(iter->second);
	}
	else
	{
		return 0;
	}
}

ShaderHandle OpenGLShaderMgr::getHandle(const std::string &name)
{
	ResHandleMap::iterator iter = mResHandleMap.find(name);
	if (iter != mResHandleMap.end())
	{
		return iter->second;
	}

	static ShaderHandle handle;
	return handle;
}

void OpenGLShaderMgr::deleteData(ShaderHandle &handle)
{
	IShader *pShader = mShaderContain.dereference(handle);
	if (pShader)
	{
		ResHandleMap::iterator iter = mResHandleMap.find(pShader->getName());
		mResHandleMap.erase(iter);
	}
	mShaderContain.release(handle);
}

IShader * OpenGLShaderMgr::_createShaderFast(const std::string &name)
{
	ShaderHandle handle;
	IShader *pShader = mShaderContain.acquire(handle);
	pShader->load(name);

	mResHandleMap.insert(std::pair<std::string, ShaderHandle>(name, handle));
	return pShader;
}

