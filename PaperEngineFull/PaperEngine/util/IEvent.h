#ifndef _I_EVENT_H_
#define _I_EVENT_H_

#if defined(_WIN32)
#include <windows.h>

class IEvent
{
public:
	static HANDLE create(
		LPSECURITY_ATTRIBUTES lpEventAttributes,
		bool bManualReset,
		bool bInitialState,
		LPCTSTR lpName)
	{
		HANDLE hEvent = ::CreateEvent(lpEventAttributes, bManualReset, bInitialState, lpName);

		if (hEvent == NULL)
		{

		}

		return hEvent;
	}

	IEvent(
		LPSECURITY_ATTRIBUTES lpEventAttributes,
		bool manualReset,
		bool initialState) :mEvent(create(lpEventAttributes, manualReset, initialState, 0)) {}

	IEvent(
		LPSECURITY_ATTRIBUTES lpEventAttributes,
		bool manualReset,
		bool initialState,
		const LPCTSTR &name) :mEvent(create(lpEventAttributes, manualReset, initialState, name)) {}

	virtual ~IEvent();

	HANDLE getEvent() const;

	void wait();

	bool wait(
		DWORD timeoutMillis);

	void reset();

	void set();

	void pulse();

private:

	HANDLE mEvent;

	// No copies do not implement
	IEvent(const IEvent &rhs);
	IEvent &operator=(const IEvent &rhs);
};

inline IEvent::~IEvent()
{
	::CloseHandle(mEvent);
}

inline HANDLE IEvent::getEvent() const
{
	return mEvent;
}

inline void IEvent::wait()
{
	if (!wait(INFINITE))
	{

	}
}

inline bool IEvent::wait(DWORD timeoutMillis)
{
	bool ok;

	DWORD result = ::WaitForSingleObject(mEvent, timeoutMillis);

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

	}

	return ok;
}

inline void IEvent::reset()
{
	if (!::ResetEvent(mEvent))
	{

	}
}

inline void IEvent::set()
{
	if (!::SetEvent(mEvent))
	{

	}
}

inline void IEvent::pulse()
{
	if (!::PulseEvent(mEvent))
	{

	}
}
#endif

class ManualResetEvent : public IEvent
{
public:
	explicit ManualResetEvent(bool init = false);
	explicit ManualResetEvent(const TCHAR *&name, bool init = false);

private:
	// No copies do not implement
	ManualResetEvent(const ManualResetEvent &rhs);
	ManualResetEvent &operator=(const ManualResetEvent &rhs);
};

inline ManualResetEvent::ManualResetEvent(bool init /*= false*/) :
IEvent(0, true, init)
{

}

inline ManualResetEvent::ManualResetEvent(const TCHAR *&name, bool init /*= false*/) :
IEvent(0, true, init, name)
{

}

#endif