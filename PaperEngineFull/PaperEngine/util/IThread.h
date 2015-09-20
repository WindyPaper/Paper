#ifndef _I_THREAD_H_
#define _I_THREAD_H_

#pragma once

#include "util/Engine_Define.h"
#include "util/Log.h"
#include <map>
#include <stdio.h>

#ifdef _WIN32
typedef uint32 ThreadId;
#else
typedef pthread_t ThreadId;
#endif

#ifdef _WIN32
#include <process.h>

class IThread
{
public:
	IThread() : mThreadHandle(INVALID_HANDLE_VALUE)	{}

	virtual ~IThread()
	{
		if (mThreadHandle != INVALID_HANDLE_VALUE)
		{
			::CloseHandle(mThreadHandle);
		}
	}

	HANDLE getHandle() const;
	void wait() const;
	bool wait(DWORD timeoutMill) const;
	void start();
	virtual int run() = 0;
	void terminate(DWORD exitcode = 0);

private:
	static uint __stdcall threadFunction(void *pV);

	HANDLE mThreadHandle;

	IThread(const IThread &rhs);
	IThread &operator=(const IThread &rhs);
};

inline HANDLE IThread::getHandle() const
{
	return mThreadHandle;
}

inline void IThread::wait() const
{
	wait(INFINITE);
}

inline bool IThread::wait(DWORD timeoutMill) const
{
	bool ok;

	DWORD result = ::WaitForSingleObject(mThreadHandle, timeoutMill);
	if (result == WAIT_TIMEOUT)
	{
		ok = false;
	}
	else if (result == WAIT_OBJECT_0)
	{
		ok = true;
	}
	else
	{
		Log::getInstance().logMsg("Thread::Wait() - WaitForSingleObject error, code = %d", ::GetLastError());
	}

	return ok;
}

inline void IThread::start()
{
	if (mThreadHandle == INVALID_HANDLE_VALUE)
	{
		uint threadId = 0;
		mThreadHandle = (HANDLE)::_beginthreadex(0, 0, threadFunction, (void*)this, 0, &threadId);
		if (mThreadHandle == INVALID_HANDLE_VALUE)
		{
			Log::getInstance().logMsg("thread error !!");
		}
	}
}

inline void IThread::terminate(DWORD exitcode /*= 0*/)
{
	if (!::TerminateThread(mThreadHandle, exitcode))
	{
		// TODO we could throw an exception here...
	}
}

inline uint __stdcall IThread::threadFunction(void *pV)
{
	int result = 0;

	IThread* pThis = (IThread*)pV;

	if (pThis)
	{
		try
		{
			result = pThis->run();
		}
		catch (...)
		{
		}
	}

	return result;
}

class CritcalSectionLock
{
public:
	CritcalSectionLock() { ::InitializeCriticalSection(&mCS); }
	~CritcalSectionLock() { ::DeleteCriticalSection(&mCS); }

	bool tryLock() { return ::TryEnterCriticalSection((CRITICAL_SECTION*)&mCS) != FALSE; }
	void lock() { ::EnterCriticalSection(&mCS); }
	void unlock() { ::LeaveCriticalSection(&mCS); }

private:
	CRITICAL_SECTION mCS;
};

class MutexLock
{
public:
	MutexLock() { mMutex = ::CreateMutex(NULL, FALSE, NULL); }
	~MutexLock() { ::CloseHandle(mMutex); }

	bool tryLock() { return ::WaitForSingleObject(mMutex, 0) != WAIT_TIMEOUT; }
	void lock() { ::WaitForSingleObject(mMutex, INFINITE); }
	void unlock() { ::ReleaseMutex(mMutex); }

private:
	HANDLE mMutex;
};

#endif

typedef std::map<uint32, void*> LockMap;

template<class T>
class LockManager
{
public:
	~LockManager()
	{
		for (LockMap::iterator iter = mLockMap.begin(); iter != mLockMap.end(); ++iter)
		{
			delete (T*)(iter->second);
		}

		mLockMap.clear();
	}

	T* getLock(uint32 lockId)
	{
		LockMap::iterator it = mLockMap.find(lockId);
		if (it != mLockMap.end())
		{
			return (T*)(it->second);
		}
		else
		{
			T* newLock = new T();
			mLockMap.insert(LockMap::value_type(lockId, newLock));
			return newLock;
		}
	}

	void deleteLock(uint32 lockId)
	{
		LockMap::iterator it = mLockMap.find(lockId);
		if (it != mLockMap.end())
		{
			delete (T*)(it->second);
			mLockMap.erase(it);
		}
	}

	static std::map< uint32, LockManager<T> > s_lockManager;
private:
	LockMap mLockMap;
};

template<class T>
std::map< uint32, LockManager<T> > LockManager<T>::s_lockManager;

template<class T> 
class ScopedLock
{
public:
	ScopedLock(uint lockGroupId, uint lockId) :
		mLockGroupId(lockGroupId),
		mLockId(lockId)
	{
		mpLock = LockManager<T>::s_lockManager[lockGroupId].getLock(lockId);
	}

	~ScopedLock()
	{
		LockManager<T>::s_lockManager[mLockGroupId].deleteLock(mLockId);
		mpLock = 0;
	}

	void lock() volatile
	{
		mpLock->lock();
	}

	void unlock() volatile
	{
		mpLock->unlock();
	}

	bool tryLock() volatile
	{
		return mpLock->tryLock();
	}

	T* getLock()
	{
		return mpLock;
	}

private:
	T* mpLock;
	uint mLockGroupId;
	uint mLockId;
};

template<class T>
class AutoLock
{
public:
	AutoLock(uint lockGroupId, uint lockId)
	{
		mpLock = LockManager<T>::s_lockManager[lockGroupId].getLock(lockId);
		mpLock->lock();
	}

	AutoLock(ScopedLock<T>* scopedLock)
	{
		mpLock = scopedLock->getLock();
		mpLock->lock();
	}

	~AutoLock()
	{
		mpLock->unlock();
	}

private:
	T* mpLock;
};

inline void EngineSleep(int millSecond)
{
#ifdef _WIN32
	::Sleep(millSecond);
#endif
}

inline ThreadId getCurrentThread()
{
#ifdef _WIN32
	return ::GetCurrentThreadId();
#endif
}

#endif
