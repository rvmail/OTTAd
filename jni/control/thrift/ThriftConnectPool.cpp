/*
 * ThriftConnectPool.cpp
 *
 *  Created on: 2014年10月30日
 *      Author: lihe
 */

#include <control/thrift/ThriftConnectPool.h>
#include "base/utils/URI.h"
#include "control/Login.h"

ThriftConnectPool::ThriftConnectPool()
{
}

ThriftConnectPool::~ThriftConnectPool()
{
}

// return NULL if error
ThriftConnection *ThriftConnectPool::getConnection()
{
	ThriftConnection *connection = NULL;

	connection = createConnection();

	return connection;
}

void ThriftConnectPool::returnConnection(ThriftConnection *connection)
{
	if (connection != NULL)
	{
		destroyConnection(connection);
	}

	return;
}

ThriftConnection *ThriftConnectPool::createConnection()
{
	URI uri(Login::getInstance()->getServerAddress("log"));
	ThriftConnection* connection = new ThriftConnection(uri.getHost(), uri.getPort());
	if (!connection->open())
	{
		destroyConnection(connection);
		return NULL;
	}

	return connection;
}

void ThriftConnectPool::destroyConnection(ThriftConnection *connection)
{
	if (connection != NULL)
	{
		connection->close();
		delete connection;
	}
}
