#ifndef _RESOURCE_H_
#define _RESOURCE_H_

#include <string>
#include <map>

#include "util/Platform.h"
#include "util/RefCount.h"
#include "util/IThread.h"

class ResourceLoader;

typedef std::map<std::string, std::string> NameParamMap;

class ENGINE_DLL Resource
{
public:
	Resource();
	Resource(const std::string &fileName);
	virtual ~Resource();

	virtual void lockRes();
	virtual void unlockRes();

	virtual bool load(ResourceLoader *pLoader) = 0;
	virtual bool load(const std::string &name) = 0;
	virtual bool load(const std::string &name, const NameParamMap &paramMap) = 0;
	virtual void unload() = 0;
	//virtual bool save(const ResourceSaver *pSaver) = 0;

	const std::string &getName() const { return mName; }
	//void setName(const std::string &name) { mName = name; }

protected:
	std::string mName;
	ScopedLock<MutexLock> *mpResLock;
};

#endif