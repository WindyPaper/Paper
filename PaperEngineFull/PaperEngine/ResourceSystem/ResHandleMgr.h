#ifndef _RES_HANDLE_MGR_H_
#define _RES_HANDLE_MGR_H_

#include <vector>
#include <assert.h>
#include "util/Platform.h"
#include "util/PreDefine.h"

template<typename DATA, typename RESHANDLE>
class ENGINE_DLL ResHandleMgr
{
public:
	ResHandleMgr();
	~ResHandleMgr();

	DATA *acquire(RESHANDLE &handle);
	void release(RESHANDLE &handle);

	DATA *dereference(RESHANDLE &handle);
	const DATA *dereference(RESHANDLE handle) const;

	uint getUsedHandleCount() const;
	bool hasUsedHandle() const;

private:
	typedef std::vector<DATA*> UserVec;
	typedef std::vector<uint> MagicVec;
	typedef std::vector<uint> FreeVec;

	UserVec mUserVec;
	MagicVec mMagicNums;
	FreeVec mFreeSlots;
};

template<typename DATA, typename RESHANDLE>
ResHandleMgr<DATA, RESHANDLE>::ResHandleMgr()
{

}

template<typename DATA, typename RESHANDLE>
ResHandleMgr<DATA, RESHANDLE>::~ResHandleMgr()
{
	for (UserVec::iterator iter = mUserVec.begin(); iter != mUserVec.end(); ++iter)
	{
		delete (*iter);
		*iter = 0;
	}
}

template<typename DATA, typename RESHANDLE>
bool ResHandleMgr<DATA, RESHANDLE>::hasUsedHandle() const
{
	return !!getUsedHandleCount();
}

template<typename DATA, typename RESHANDLE>
uint ResHandleMgr<DATA, RESHANDLE>::getUsedHandleCount() const
{
	return mMagicNums.size() - mFreeSlots.size()
}

template<typename DATA, typename RESHANDLE>
const DATA * ResHandleMgr<DATA, RESHANDLE>::dereference(RESHANDLE handle) const
{
	typedef ResHandleMgr<DATA, RESHANDLE> ThisType;
	return const_cast<ThisType*>(this)->dereference(handle);
}

template<typename DATA, typename RESHANDLE>
DATA * ResHandleMgr<DATA, RESHANDLE>::dereference(RESHANDLE &handle)
{
	if (handle.isNull())
	{
		return 0;
	}

	uint index = handle.getIndex();
	if (index >= mUserVec.size() || mMagicNums[index] != handle.getMagic())
	{
		assert(0);
		return 0;
	}

	/*UserVec::iterator iter = mUserVec.begin() + index;
	return *iter;*/
	return mUserVec[index];
}

template<typename DATA, typename RESHANDLE>
void ResHandleMgr<DATA, RESHANDLE>::release(RESHANDLE &handle)
{
	uint index = handle.getIndex();

	assert(index < mUserVec.size());
	assert(mMagicNums[index] == handle.getMagic());

	mUserVec[index]->unload();
	mMagicNums[index] = 0;
	mFreeSlots.push_back(index);
}

template<typename DATA, typename RESHANDLE>
DATA * ResHandleMgr<DATA, RESHANDLE>::acquire(RESHANDLE &handle)
{
	uint index;
	if (mFreeSlots.empty())
	{
		index = mMagicNums.size();
		handle.init(index);
		DATA *pData = new DATA();
		mUserVec.push_back(pData);
		mMagicNums.push_back(handle.getMagic());
	}
	else
	{
 		index = mFreeSlots.back();
		handle.init(index);
		mFreeSlots.pop_back();
		mMagicNums[index] = handle.getMagic();
	}

	//return (mUserVec.begin() + index);
	UserVec::iterator iter = mUserVec.begin() + index;
	return *iter;
}



#endif