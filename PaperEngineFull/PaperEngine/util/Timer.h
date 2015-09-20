#ifndef _TIMER_H_
#define _TIMER_H_

#include "util/Engine_Define.h"

class Timer
{
public:
	Timer();
	~Timer();

	void start();
	void end();

	double getCurrTime() const;
	double getOffsetTime() const;

	void reset();
	      
protected:
	EINT64 getInternalTime() const; 

private:
	unsigned long mTimeMask;
	EINT64 mStartTime;
	EINT64 mEndTime;
};


#endif