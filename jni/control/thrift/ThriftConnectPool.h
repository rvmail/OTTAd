/*
 * ThriftConnectPool.h
 *
 *  Created on: 2014年10月30日
 *      Author: lihe
 */

#ifndef THRIFTCONNECTPOOL_H_
#define THRIFTCONNECTPOOL_H_

#include "base/utils/log.h"
#include <vector>
#include <mutex>
#include <queue>

#include "scribe.h"
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include "scribe_types.h"
#include "scribe_constants.h"

using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

using boost::shared_ptr;

#define DEFAULT_NETWORK_TIMEOUT 0

class ThriftConnection
{
	friend class ThriftConnectPool;
public:
	ThriftConnection(const std::string host, const int port) :
		socket(new TSocket(host, port)),
		transport(new TFramedTransport(socket)),
		protocol(new TBinaryProtocol(transport)),
		client(new scribe::thrift::scribeClient(protocol))
    {
    }

	bool send(const std::string &category, const std::string& message)
	{
		try
		{
			std::vector<scribe::thrift::LogEntry> LogArr;

			scribe::thrift::LogEntry logEntry;
			logEntry.category = category;
			logEntry.message = message;
			LOG(DEBUG) << "category : " << logEntry.category;
			LOG(DEBUG) << "message : " << logEntry.message;

			LogArr.push_back(logEntry);

			client->send_Log(LogArr);

			LOG(DEBUG) << "ThriftConnection send finish";
			return true;
		} catch (TException& tx) {
			LOG(ERROR) << "ERROR: " << tx.what();
			return false;
		}
	}

private:
	bool open()
	{
		try
		{
			transport->open();
			return true;
		} catch (TException& tx) {
			LOG(ERROR) << "ERROR: " << tx.what();
			return false;
		}
	}

	void close()
	{
		transport->close();
	}

private:
    boost::shared_ptr<TSocket> socket;
    boost::shared_ptr<TTransport> transport;
    boost::shared_ptr<TProtocol> protocol;
    boost::shared_ptr<scribe::thrift::scribeClient> client;
};

class ThriftConnectPool
{
public:
	ThriftConnectPool();
	virtual ~ThriftConnectPool();

	ThriftConnection *getConnection();
	void returnConnection(ThriftConnection *connection);

private:
	ThriftConnection *createConnection();
	void destroyConnection(ThriftConnection *connection);
};

#endif /* THRIFTCONNECTPOOL_H_ */
