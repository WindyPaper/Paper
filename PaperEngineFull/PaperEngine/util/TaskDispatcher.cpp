#include "util/PrecompileHead.h"
#include "util/TaskDispatcher.h"

CritcalSectionLock g_lock;
CritcalSectionLock g_finishLock;

TaskThread::TaskThread(int threadId, TaskDispatcher *pCreator) :
mpCreator(pCreator),
mThreadId(threadId)
{
	mpRunningFlag = new ManualResetEvent(false);
	mpWaitFlag = new ManualResetEvent(false);
	mIsRunning = false;
	mProcessedSinceLastQuery = 0;
}

TaskThread::~TaskThread()
{
	SAFE_DELETE(mpRunningFlag);
	SAFE_DELETE(mpWaitFlag);
}

int TaskThread::run()
{
	while (true)
	{
		mpWaitFlag->wait();
		mpWaitFlag->reset();
		mIsRunning = true;

		while (true)
		{
			ITask* task = mpCreator->requestTask();
			if (!task)
			{
				// 没有任务了，退出线程
				mIsRunning = false;
				break;
			}
			// 执行任务
			task->realExecute();
			// 放入已完成队列
			mpCreator->finishTask(task);
			//delete task;
			mProcessedSinceLastQuery++;
		}
		mpRunningFlag->set();
	}

	return 0;
}

void TaskThread::setReady()
{
	mpRunningFlag->reset();
	mpWaitFlag->set();
}

HANDLE TaskThread::getWaitingHandle()
{
	return mpRunningFlag->getEvent();
}

IEvent * TaskThread::getWaitingEvent()
{
	return mpRunningFlag;
}

ThreadStatus TaskThread::queryStatus()
{
	ThreadStatus status;
	status.running = mIsRunning;
	status.processedTask = mProcessedSinceLastQuery;
	mProcessedSinceLastQuery = 0;
	return status;
}

//--------------------------------------------------------------

TaskDispatcher::TaskDispatcher()
{

}

TaskDispatcher::~TaskDispatcher()
{
	destory();
}

void TaskDispatcher::init()
{
	int threadCount = 4;

	for (int i = 0; i < threadCount; ++i)
	{
		TaskThread *pThisThread = new TaskThread(i, this);

		mThreadPool.push_back(pThisThread);

		uint core = i + 1;

#if defined(_WIN32)
		DWORD_PTR mask1, mask2;
		GetProcessAffinityMask(GetCurrentProcess(), &mask1, &mask2);
		if (BIT(core) & mask1)	// Check if we have this affinity
			SetThreadAffinityMask(pThisThread->getHandle(), BIT(core));
		else // Reserve CPU 1 for main thread.
			SetThreadAffinityMask(pThisThread->getHandle(), (mask1 & (~1)));
#endif
	}
	
	for (TaskThreadPool::iterator iter = mThreadPool.begin(); iter != mThreadPool.end(); ++iter)
	{
		(*iter)->start();
	}
}


void TaskDispatcher::destory()
{
	for (TaskThreadPool::iterator iter = mThreadPool.begin(); iter != mThreadPool.end(); ++iter)
	{
		(*iter)->terminate();
		SAFE_DELETE(*iter);
	}
}

void TaskDispatcher::cancelTask(ITask *pTask)
{
	g_lock.lock();

	for (TaskList::iterator iter = mTaskList.begin(); iter != mTaskList.end(); ++iter)
	{
		if (pTask == (*iter))
		{
			pTask->cancel();
			break;
		}
	}

	g_lock.unlock();
}

void TaskDispatcher::pushTask(ITask *pTask, int priority)
{
	if (priority == -2)
	{
		pTask->execute();
		pTask->complete();
		delete pTask;

		return;
	}

	g_lock.lock();

	if (priority == -1)
	{
		mTaskStack.push_front(pTask);
	}
	else
	{
		mTaskStack.push_back(pTask);
	}
	mTaskList.push_back(pTask);

	flush();
	g_lock.unlock();
}

void TaskDispatcher::flushCoop()
{
	flush();

	while (true)
	{
		ITask *pTask = requestTask();
		if (!pTask)
		{
			break;
		}
		pTask->execute();
	}
}

void TaskDispatcher::flush()
{
	for (TaskThreadPool::iterator iter = mThreadPool.begin(); iter != mThreadPool.end(); ++iter)
	{
		(*iter)->setReady();
	}
}

void TaskDispatcher::wait()
{
	std::vector<HANDLE> handles;
	for (TaskThreadPool::iterator iter = mThreadPool.begin(); iter != mThreadPool.end(); ++iter)
	{
		handles.push_back((*iter)->getWaitingHandle());

		(*iter)->getWaitingEvent()->wait();
	}
}

TaskStatus TaskDispatcher::queryTaskStatus()
{
	TaskStatus status;

	status.threadCount = mThreadPool.size();
	status.taskTotal = mTaskStack.size();
	for (uint32 i = 0; i < mThreadPool.size(); ++i)
	{
		ThreadStatus tdStatus = mThreadPool[i]->queryStatus();
		if (tdStatus.running)
		{
			status.taskNum[i] = 1;
			status.taskTotal++;
		}
	}

	return status;
}


void TaskDispatcher::update()
{
	while (true)
	{
		if (mTaskFinishedStack.empty())
		{
			break;
		}

		ITask *pThisTask = 0;
		g_finishLock.lock();

		pThisTask = mTaskFinishedStack.front();
		mTaskFinishedStack.pop_front();

		g_finishLock.unlock();

		if (pThisTask)
		{
			pThisTask->complete();
			delete pThisTask;
		}
	}
}

ITask * TaskDispatcher::requestTask()
{
	ITask* ret = NULL;

	g_lock.lock();
	if (!mTaskStack.empty())
	{
		ret = mTaskStack.front();
		mTaskStack.pop_front();
	}
	if (!mTaskList.empty())
	{
		TaskList::iterator it = mTaskList.begin();
		for (; it != mTaskList.end(); ++it)
		{
			if (ret == *it)
			{
				mTaskList.erase(it);
				break;
			}
		}
	}

	g_lock.unlock();

	return ret;
}

void TaskDispatcher::finishTask(ITask *pTask)
{
	g_finishLock.lock();
	mTaskFinishedStack.push_back(pTask);
	g_finishLock.unlock();
}
