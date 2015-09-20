#include "util/PrecompileHead.h"
#include "util/Timer.h"

Timer::Timer() :
	mTimeMask(0),
	mStartTime(0.0),
	mEndTime(0.0)
{
	//找到合适的计算计时器的核
	DWORD_PTR procMask;
	DWORD_PTR sysMask;
	GetProcessAffinityMask(GetCurrentProcess(), &procMask, &sysMask);

	if(procMask == 0)
	{
		procMask = 1;
	}

	if(mTimeMask == 0)
	{
		mTimeMask = 1;
		while((mTimeMask & procMask) == 0)
		{
			mTimeMask <<= 1;
		}
	}
}

Timer::~Timer()
{

}


void Timer::start()
{
	mStartTime = getCurrTime();
}

void Timer::end()
{
	mEndTime = getCurrTime();
}

double Timer::getCurrTime() const
{
	HANDLE thread = GetCurrentThread();
	DWORD_PTR oldMask = SetThreadAffinityMask(thread, mTimeMask);
	EINT64 currTime = this->getInternalTime();
	EINT64 freq;
	QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
	double seconds = currTime* 1000 / freq;
	SetThreadAffinityMask(thread, oldMask);
	return seconds;
}

double Timer::getOffsetTime() const
{
	return mEndTime - mStartTime;
}

EINT64 Timer::getInternalTime() const
{
	EINT64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

	return currTime;
}