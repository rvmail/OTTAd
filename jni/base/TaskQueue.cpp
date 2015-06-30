/*
 * TaskQueue.cpp
 *
 *  Created on: 2014年10月28日
 *      Author: lihe
 */

#include <base/TaskQueue.h>
#include <base/utils/log.h>
#include <unistd.h>

std::mutex TaskQueue::mtx;
std::condition_variable TaskQueue::cv;

TaskQueue::TaskQueue()
{
	threadCount = 1;
	setIsStart(false);
}

TaskQueue::TaskQueue(int count)
{
	threadCount = count;
	setIsStart(false);
}

TaskQueue::~TaskQueue()
{
	setIsStart(false);
}

void TaskQueue::start()
{
	setIsStart(true);

	for (int i = 0; i < threadCount; i++)
	{
		threads.push_back(std::thread(loop, this, i));
	}
}

void TaskQueue::quit()
{
	setIsStart(false);
	cv.notify_all();
	for (auto& th : threads) th.join();
}

void TaskQueue::loop(void *self, int id)
{
  TaskQueue *taskQueue = static_cast<TaskQueue *>(self);
  TaskMessage* taskMessage = NULL;

  while(taskQueue->getIsStart())
  {
	  taskMessage = taskQueue->getTaskMessage();

	  if (taskMessage == NULL)
	  {
		  LOG(ERROR) << "Message is null";
		  continue;
	  }
	  LOG(DEBUG) << "thread " << id << " handle this message";

	  taskQueue->handleMessage(taskMessage);

	  delete taskMessage;
  }
}

bool TaskQueue::getIsStart()
{
	return isStart;
}

void TaskQueue::setIsStart(bool start)
{
	isStart = start;
}

MSQ_QUEUE& TaskQueue::getQueue()
{
	return mQueue;
}

TaskMessage *TaskQueue::getTaskMessage()
{
	TaskMessage* taskMessage = NULL;

	std::unique_lock < std::mutex > lck(mtx);

	while (mQueue.empty())
	{
		if (!getIsStart())
		{
			return NULL;
		}
		cv.wait(lck);
	}

	taskMessage = mQueue.front();
	mQueue.pop();

	return taskMessage;
}

void TaskQueue::sendMessage(TaskMessage *taskMessage)
{
	if(taskMessage == NULL)
	{
		LOG(ERROR) << "Message is null";
		return;
	}

	std::unique_lock < std::mutex > lck(mtx);
	mQueue.push(taskMessage);

	cv.notify_one();
}
