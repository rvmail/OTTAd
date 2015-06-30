/*
 * TaskQueue.h
 *
 *  Created on: 2014年10月28日
 *      Author: lihe
 */

#ifndef TASKQUEUE_H_
#define TASKQUEUE_H_

#include <thread>
#include <mutex>
#include <condition_variable>

#include <queue>

class TaskMessage
{
public:
	~TaskMessage() {};
	TaskMessage()
	{
		type = -1;
		message = NULL;
	};

public:
	int type;
	void* message;
};

typedef std::queue<TaskMessage*> MSQ_QUEUE;

class TaskQueue
{
public:
	static std::mutex mtx;
	static std::condition_variable cv;

public:
	TaskQueue();
	TaskQueue(int count);
	virtual ~TaskQueue();

	void start();
	void quit();

	virtual void handleMessage(TaskMessage *taskMessage) = 0;
	void sendMessage(TaskMessage *taskMessage);
	//void sendMessageDelayed(TaskMessage *taskMessage, long delayMillis);

private:
	static void loop(void *self, int id);
	bool getIsStart();
	void setIsStart(bool start);
	MSQ_QUEUE& getQueue();
	TaskMessage *getTaskMessage();

	bool isStart;
	MSQ_QUEUE mQueue;
	int threadCount;
	std::vector<std::thread> threads;
};

#endif /* TASKQUEUE_H_ */
