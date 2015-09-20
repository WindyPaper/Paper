#include "util/PrecompileHead.h"
#include "util/Engine_Define.h"
#include "ResourceSystem/Resource.h"
#include "ResourceSystem/ResourceLoader.h"

Resource::Resource()
{
	mpResLock = new ScopedLock<MutexLock>(0, (uint)this);
}

Resource::Resource(const std::string &fileName) :
mName(fileName)
{
	mpResLock = new ScopedLock<MutexLock>(0, (uint)this);
}

Resource::~Resource()
{
	SAFE_DELETE(mpResLock);
}

void Resource::lockRes()
{
	mpResLock->lock();
}

void Resource::unlockRes()
{
	mpResLock->unlock();
}
