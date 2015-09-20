#ifndef _TEXTURE_MGR_H_
#define _TEXTURE_MGR_H_

#include <string>
#include <map>

#include "util/Platform.h"
#include "ResourceSystem/ResHandle.h"
#include "ResourceSystem/ResHandleMgr.h"
#include "ResourceSystem/ITexture.h"
#include "ResourceSystem/IResMgr.h"
#include "OpenGLPlatform/OpenGLTexture.h"


class ENGINE_DLL TextureMgr : public ITextureMgr
{
public:
	
	typedef ResHandleMgr<OpenGLTexture, TexHandle> ImageContain;

	struct IsStringLess
	{
		bool operator() (const std::string &l, const std::string &r) const
		{
			return (::stricmp(l.c_str(), r.c_str()) < 0);
		}
	};
	typedef std::map<std::string, TexHandle, IsStringLess> ResNameMap;
	typedef std::pair<ResNameMap::iterator, bool> ResNameMapInsertRc;

	TextureMgr();
	~TextureMgr();

	ITexture *create(const std::string &name);
	ITexture *create(const std::string &name, const NameParamMap &paramMap);

	TexHandle getHandle(const std::string &name);
	ITexture *getDataPtr(const std::string &name);
	ITexture *getDataPtr(TexHandle &handle);

	void deleteData(TexHandle &handle);

private:
	ImageContain mImageContain;

	ResNameMap mResNameMap;
};

#endif