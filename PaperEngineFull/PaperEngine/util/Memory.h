#ifndef _MEMORY_H_
#define _MEMORY_H_

#include "util/SmartPtr.h"

namespace Memory
{
	template<typename T>
	void createSmartPtrIns(SmartPtr<T> &ptr)
	{
		ptr = new T();
		//return ptr;
	}
}

#endif