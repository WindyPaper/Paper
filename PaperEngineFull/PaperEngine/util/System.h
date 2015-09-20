#ifndef _SYSTEM_H_
#define _SYSTEM_H_

class Log;

class System
{
public:
	System();
	~System();

	void init();

private:
	Log *pLog;
};


#endif