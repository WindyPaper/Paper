#ifndef _RES_HANDLE_H_
#define _RES_HANDLE_H_

#include <assert.h>
#include <map>
#include "util/Platform.h"
#include "util/PreDefine.h"
#include "util/Log.h"

template<typename ResType>
class ENGINE_DLL ResHandle
{
public:
	ResHandle();
	ResHandle(const ResHandle &handle);

	~ResHandle();

	void init(uint index);
	uint getIndex() const;
	uint getMagic() const;
	uint getHandle() const;

	bool isNull() const;

	operator uint() const;

	void addRef();
	
	void release();

	ResHandle &operator =(ResHandle handle)
	{
		/*if (handle.isNull() == false)			
		{
			assert(handle.mpRefCount != 0);
			if (isNull() == false && mpRefCount)
			{
				//release();
				//if (mpRefCount != 0)
					//assert(mpRefCount == handle.mpRefCount);
			}
		}*/
		if (isNull() == false && mpRefCount)
		{
			release();
			//if (mpRefCount != 0)
				//assert(mpRefCount == handle.mpRefCount);
		}

		mHandle = handle.getHandle();
		mpRefCount = handle.mpRefCount;
		addRef();
		return *this;
	}

private:
	union
	{
		enum
		{
			MAX_BITS_INDEX = 16,
			MAX_BITS_MAGIC = 16,

			MAX_INDEX = (1 << MAX_BITS_INDEX) - 1,
			MAX_MAGIC = (1 << MAX_BITS_MAGIC) - 1,
		};

		struct
		{
			unsigned mIndex : MAX_BITS_INDEX;
			unsigned mMagic : MAX_BITS_MAGIC;
		};

		uint mHandle;
	};

	uint *mpRefCount;
};

template<typename ResType>
ResHandle<ResType>::ResHandle(const ResHandle &handle) :
mpRefCount(0)
{
	mHandle = handle.getHandle();
	mpRefCount = handle.mpRefCount;
	addRef();
}

template<typename ResType>
ResHandle<ResType>::~ResHandle()
{
	release();
}

template<typename ResType>
void ResHandle<ResType>::release()
{
	if (mHandle != 0)
	{
		--(*mpRefCount);
		if (*mpRefCount == 0)
		{
			//std::cout << "need delete !!!!" << std::endl;
			//delete operator
			//后面加入
			Log::getInstancePtr()->logMsg("release res handle = %d", mHandle);

			SAFE_DELETE(mpRefCount);
		}
	}
}

template<typename ResType>
void ResHandle<ResType>::addRef()
{
	if (mHandle != 0)
	{
		++(*mpRefCount);
	}
}

template<typename ResType>
ResHandle<ResType>::ResHandle() :
mHandle(0),
mpRefCount(0)
{

}

template<typename ResType>
void ResHandle<ResType>::init(uint index)
{
	assert(isNull());
	assert(index <= MAX_INDEX);

	static uint sAutoMagic = 0;

	if (++sAutoMagic > MAX_MAGIC)
	{
		sAutoMagic = 1;
	}

	mIndex = index;
	mMagic = sAutoMagic;

	if (mpRefCount == 0)
	{
		mpRefCount = new uint;		
	}
	*mpRefCount = 1;
}

template<typename ResType>
ResHandle<ResType>::operator uint() const
{
	return mHandle;
}

template<typename ResType>
bool ResHandle<ResType>::isNull() const
{
	return !mHandle;
}

template<typename ResType>
uint ResHandle<ResType>::getHandle() const
{
	return mHandle;
}

template<typename ResType>
uint ResHandle<ResType>::getMagic() const
{
	return mMagic;
}

template<typename ResType>
uint ResHandle<ResType>::getIndex() const
{
	return mIndex;
}

//-------------------------------------------------------------------

template < typename Tag>
inline bool operator != (ResHandle<Tag> l, ResHandle<Tag> r)
{
	return (l.getHandle() != r.getHandle());
}


template < typename Tag>
inline bool operator == (ResHandle<Tag> l, ResHandle<Tag> r)
{
	return (l.getHandle() == r.getHandle());
}

#endif