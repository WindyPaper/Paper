#ifndef _SINGLE_TON_H_
#define _SINGLE_TON_H_

#include <assert.h>
#include "util/Platform.h"

template<class T>
class ENGINE_DLL Singleton
{
private:
	/** \brief Explicit private copy constructor. This is a forbidden operation.*/
	Singleton(const Singleton<T> &);

	/** \brief Private operator= . This is a forbidden operation. */
	Singleton& operator=(const Singleton<T> &);

protected:
	static T* msInstance;

public:
	Singleton(void)
	{
		assert(!msInstance);
#if defined( _MSC_VER ) && _MSC_VER < 1200	 
		int offset = (int)(T*)1 - (int)(Singleton <T>*)(T*)1;
		msInstance = (T*)((int)this + offset);
#else
		msInstance = static_cast<T*>(this);
#endif
	}
	~Singleton(void)
	{
		assert(msInstance);  msInstance = 0;
	}
	static T& getInstance(void)
	{
		assert(msInstance);  return (*msInstance);
	}
	static T* getInstancePtr(void)
	{
		return msInstance;
	}
};

#endif