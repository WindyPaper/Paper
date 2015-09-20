#include "OpenGLPlatform/RenderPreCompileHead.h"
#include "OpenGLPlatform/OpenGLTextureMgr.h"

TextureMgr::TextureMgr()
{

}

TextureMgr::~TextureMgr()
{
	for (ResNameMap::iterator iter = mResNameMap.begin(); iter != mResNameMap.end(); ++iter)
	{
		mImageContain.dereference(iter->second)->unload();
	}
}

ITexture * TextureMgr::create(const std::string &name)
{
	TexHandle handle;
	ResNameMapInsertRc rc = mResNameMap.insert(std::pair<std::string, TexHandle>(name, handle));
	if (rc.second)
	{
		ITexture *pTexture = mImageContain.acquire(rc.first->second);
		if (!pTexture->load(name))
		{
			deleteData(rc.first->second);
			rc.first->second = TexHandle();
		}
	}
	return mImageContain.dereference(rc.first->second);
}

/*
ITexture * TextureMgr::create(const std::string &name, const std::string &fileName)
{
	TexHandle handle;
	ResNameMapInsertRc rc = mResNameMap.insert(std::pair<std::string, TexHandle>(name, handle));
	if (rc.second)
	{
		ITexture *pTexture = mImageContain.acquire(rc.first->second);
		if (!pTexture->load(fileName))
		{
			deleteData(rc.first->second);
			rc.first->second = TexHandle();
		}
	}
	return mImageContain.dereference(rc.first->second);
}*/

ITexture * TextureMgr::create(const std::string &name, const NameParamMap &paramMap)
{
	TexHandle handle;
	ResNameMapInsertRc rc = mResNameMap.insert(std::pair<std::string, TexHandle>(name, handle));
	if (rc.second)
	{
		mImageContain.acquire(rc.first->second);
	}
	return mImageContain.dereference(rc.first->second);
}

TexHandle TextureMgr::getHandle(const std::string &name)
{
	TexHandle handle;
	ResNameMapInsertRc rc = mResNameMap.insert(std::pair<std::string, TexHandle>(name, handle));
	if (rc.second)
	{
		ITexture *pTexture = mImageContain.acquire(rc.first->second);
		if (!pTexture->load(rc.first->first))
		{
			deleteData(rc.first->second);
			rc.first->second = TexHandle();
		}
	}
	return rc.first->second;
}

ITexture * TextureMgr::getDataPtr(const std::string &name)
{
	TexHandle handle = getHandle(name);
	return mImageContain.dereference(handle);
}

ITexture * TextureMgr::getDataPtr(TexHandle &handle)
{
	return mImageContain.dereference(handle);
}

void TextureMgr::deleteData(TexHandle &handle)
{
	ITexture *pRes = mImageContain.dereference(handle);
	if (pRes)
	{
		mResNameMap.erase(mResNameMap.find(pRes->getName()));
		//pRes->unload();

		mImageContain.release(handle);
	}
}
