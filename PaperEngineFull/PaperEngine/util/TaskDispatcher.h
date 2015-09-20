#ifndef _TASK_DISPATCHER_H_
#define _TASK_DISPATCHER_H_

#include <vector>
#include <queue>
#include "util/ITask.h"
#include "util/IThread.h"
#include "util/IEvent.h"

class TaskDispatcher;

struct ThreadStatus
{
	bool running;
	int processedTask;
};

class TaskThread : public IThread
{
public:
	TaskThread(int threadId, TaskDispatcher *pCreator);
	~TaskThread();

	virtual int run();
	void setReady();
	HANDLE getWaitingHandle();
	IEvent *getWaitingEvent();
	ThreadStatus queryStatus();

private:
	int mThreadId;

	ManualResetEvent *mpRunningFlag;
	ManualResetEvent *mpWaitFlag;

	TaskDispatcher *mpCreator;

	bool mIsRunning;
	int mProcessedSinceLastQuery;
};

typedef std::vector<TaskThread*> TaskThreadPool;
typedef std::deque<ITask*> TaskStack;
typedef std::vector<ITask*> TaskList;

class TaskDispatcher : public ITaskDispatcher
{
public:
	TaskDispatcher();
	~TaskDispatcher();

	void init();
	void destory();

	void cancelTask(ITask *pTask);
	void pushTask(ITask *pTask, int priority);
	void flushCoop();
	void flush();
	void wait();
	virtual TaskStatus queryTaskStatus();

	void update();
	ITask *requestTask();
	void finishTask(ITask *pTask);

private:
	TaskThreadPool mThreadPool;
	TaskStack mTaskStack;
	TaskList mTaskList;
	TaskStack mTaskFinishedStack;
};

#endif