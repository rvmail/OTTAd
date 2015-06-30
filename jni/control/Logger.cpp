#include "Logger.h"
#include "base/utils/log.h"
#include "base/utils/DeviceInfo.h"
#include "base/TaskQueue.h"
#include "base/dataCache.h"
#include "control/Login.h"
#include "thrift/ThriftConnectPool.h"

#include <Poco/Util/Timer.h>
#include <Poco/Util/TimerTask.h>
#include <Poco/Util/TimerTaskAdapter.h>

using boost::shared_ptr;

class LoggerTaskQueue : public TaskQueue
{
private:
	typedef vector<LoggerRecord *> RecordVector;

    boost::shared_ptr<ThriftConnectPool> connectPool;
    RecordVector Records;
	Poco::Util::Timer timer;
	Poco::Util::TimerTask::Ptr pTask;

public:
	static mutex rv_mtx;

public:
    LoggerTaskQueue() : TaskQueue(5),
		connectPool(new ThriftConnectPool())
    {
    	pTask = new Poco::Util::TimerTaskAdapter<LoggerTaskQueue>(*this, &LoggerTaskQueue::onTimer);
		timer.schedule(pTask, 1000 * 5, 1000 * 5);
    }

    ~LoggerTaskQueue()
    {
		pTask->cancel();
    }

	void handleMessage(TaskMessage *taskMessage)
	{
		LOG(DEBUG) << "handleMessage : " << taskMessage->type;
		if (taskMessage == NULL)
		{
			LOG(ERROR) << "handleMessage: Message is null";
			return;
		}

		switch (taskMessage->type) {
			case loggerTypeRealTime:
				sendRealTimeLog(taskMessage->message);
				break;
			case loggerTypeNoRealTime:
				addNoRealTimeLog(taskMessage->message);
				break;
			case loggerTypeSendNoRealTime:
				sendNoRealTimeLog();
				break;
			default:
				break;
		}
	}

private:
	void sendRealTimeLog(void *record)
	{
		LoggerRecord *pRecord = static_cast<LoggerRecord *>(record);
		LOG(DEBUG) << "sendRealTimeLog";

		string log = getLoggerHead() + pRecord->toString();
		LOG(DEBUG) << "sendRealTimeLog : " << log;

		bool success = false;
		for (int retry = 0; retry < 4; retry++)
		{
			if (retry > 0)
				LOG(DEBUG) << "Send real time log failed, retry " << retry;

			ThriftConnection *connection = connectPool->getConnection();
			if (connection != NULL)
			{
				success = connection->send("REALTIME", log);
				connectPool->returnConnection(connection);
				if (success) break;
			}
		}

		delete pRecord;
	}

	void sendNoRealTimeLog()
	{
		LOG(DEBUG) << "sendNoRealTimeLog";

		RecordVector sendRecords;
		rv_mtx.lock();
		if (!Records.empty())
		{
			sendRecords.assign(Records.begin(), Records.end());
			Records.clear();
			rv_mtx.unlock();
		} else {
			rv_mtx.unlock();
			LOG(DEBUG) << "Record queue is empty";
			return;
		}

		LoggerRecord *pRecord = NULL;
		string body;
        for (RecordVector::iterator it = sendRecords.begin(); it != sendRecords.end(); it++)
        {
            pRecord = (LoggerRecord *)*it;
            if (pRecord != NULL)
            {
            	body += pRecord->toString();
            	if ((it + 1) != sendRecords.end())
            	{
            		body += "`";
            	}

            	delete pRecord;
            }
        }

        string log = getLoggerHead() + body;

		LOG(DEBUG) << "sendNoRealTimeLog : " << log;

		bool success = false;
		for (int retry = 0; retry < 4; retry++)
		{
			if (retry > 0)
				LOG(DEBUG) << "Send real time log failed, retry " << retry;

			ThriftConnection *connection = connectPool->getConnection();
			if (connection != NULL)
			{
				success = connection->send("NOREALTIME", log);
				connectPool->returnConnection(connection);
				if (success) break;
			}
		}
	}

	void addNoRealTimeLog(void *record)
	{
		LoggerRecord *pRecord = static_cast<LoggerRecord *>(record);

		rv_mtx.lock();
		Records.push_back(pRecord);
		rv_mtx.unlock();
	}

	// head formate:      icntvId|mac|platformatId|version|ip|uploadTime
	string getLoggerHead()
	{
		LOG(DEBUG) << "getLoggerHead";

		Login *login = Login::getInstance();
		stringstream head;
		head << login->getDeviceID();
		head << "|" << getMacInNum();
		head << "|" << login->getPlatformID();
		head << "|" << dataCache::getInstance()->getVersion();
		head << "|" << "{0}";
		head << "|" << SystemClock::currentTimeMillis();
		head << "|";

		return head.str();
	}

	void onTimer(Poco::Util::TimerTask& task)
	{
		TaskMessage* taskMessage = new TaskMessage();
		taskMessage->type = loggerTypeSendNoRealTime;
		sendMessage(taskMessage);
	}
};
mutex LoggerTaskQueue::rv_mtx;

Logger* Logger::m_pInstance = NULL;

Logger* Logger::getInstance()
{
	if (m_pInstance == NULL)
	{
		m_pInstance = new Logger;
	}

	return m_pInstance;
}

Logger::Logger(void)
{
	loggerTQ = new LoggerTaskQueue();
	loggerTQ->start();
}

Logger::~Logger(void)
{
	loggerTQ->quit();
	delete loggerTQ;
}

void Logger::recode(char* log)
{
}

void Logger::sendLog(LoggerType type, Module module, Action action, const string &content)
{
	LoggerRecord *record = new LoggerRecord(module, action, content);

	TaskMessage* taskMessage = new TaskMessage();
	taskMessage->type = type;
	taskMessage->message = (void *)record;

	loggerTQ->sendMessage(taskMessage);
}
