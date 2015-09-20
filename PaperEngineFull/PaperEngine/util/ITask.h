#ifndef _I_TASK_H_
#define _I_TASK_H_

#include <memory.h>

class ITask
{
public:
	ITask() : mScheduled(true) {}
	virtual ~ITask() {}

	void cancel() { mScheduled = false; }
	void realExecute() { if (mScheduled) execute(); }
	virtual void execute() = 0;
	virtual void complete() {}

private:
	volatile bool mScheduled;
};

struct TaskStatus
{
	int threadCount;
	int taskTotal;
	int taskNum[8];

	TaskStatus()
	{
		memset(this, 0, sizeof(TaskStatus));
	}
};

class ITaskDispatcher
{
public:
	virtual ~ITaskDispatcher() {}

	virtual void init() = 0;
	virtual void destory() = 0;
	virtual void cancelTask(ITask *pTask) = 0;
	virtual void pushTask(ITask *pTask, int priority) = 0;
	virtual void pushTask(ITask *pTask) { pushTask(pTask, 0); }
	virtual void flushCoop() = 0;
	virtual void flush() = 0;
	virtual void wait() = 0;
	virtual void update() = 0;

	virtual TaskStatus queryTaskStatus() = 0;
};

#endif