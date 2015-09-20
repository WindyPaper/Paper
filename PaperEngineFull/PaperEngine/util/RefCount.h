#ifndef _REF_COUNT_H_
#define _REF_COUNT_H_

#include <assert.h>

class RefCount 
{
public:
	virtual void addRef();
	virtual void release();

	size_t getRefCount();

protected:
	RefCount() : mRefCount(0)
	{

	}

	virtual ~RefCount()
	{
		assert(mRefCount == 0);
	}

private:
	volatile size_t mRefCount;
};

inline void RefCount::addRef()
{
	// not thread safe
	++mRefCount;
}

inline void RefCount::release()
{
	assert(mRefCount > 0);
	--mRefCount;
	if(mRefCount == 0)
	{
		delete this;
	}
}

inline size_t RefCount::getRefCount()
{
	return mRefCount;
}

#endif