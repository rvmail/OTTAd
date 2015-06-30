/**
*
* Copyright ${YEAR}$ Future TV, Inc.
*
 * The contents of this file are subject to the terms
 * of the Common Development and Distribution License
 * (the License). You may not use this file except in
 * compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.icntv.tv/licenses/LICENSE-1.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
*
* @author   chu.lei@icntv.tv
* @date     2014-9-19
*/
/************************************************************************/
/* ����˵������־����
*/
/************************************************************************/
#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <string>
#include <sstream>
#include <vector>
#include <mutex>

#include "base/utils/SystemClock.h"

using namespace std;

class LoggerTaskQueue;

enum LoggerType
{
	unknow,
	loggerTypeRealTime,
	loggerTypeNoRealTime,
	loggerTypeSendNoRealTime,
};

enum Module
{
	moduleStart = 1,
	moduleAuth,
	moduleUser,
	modulePlay,
	moduleCdn,
};

enum Action
{
	actionStartup = 0,
	actionShutdown = 1,

	actionActivate = 0,
	actionAuth = 1,

	actionBrowser = 0,
	actionPay = 1,
	actionPrice = 2,
	actionSearch = 3,
	actionPlayBack = 4,
	actionCollect = 5,
	actionRecommend = 6,
	actionSearchResult = 7,

	actionPlay = 0,
	actionSeek  = 1,
	actionPause = 2,
	actionCache = 3,
	actionStop = 4,
	actionQuality = 5,

	actionDownloadTask = 0,
	actionSchedule = 1,
};

class LoggerRecord
{
public:
    LoggerRecord(const Module module, const Action action, const string &content){
        mModule = module;
        mAction = action;
        mContent = content;
        mLogCreateTime = SystemClock::currentTimeMillis();
    }

    string toString()
    {
		stringstream ss;
		ss << "[";
		ss << mLogCreateTime;
		ss << "|" << mModule;
		ss << "|" << mAction;
		ss << "|" << mContent;
		ss << "]";

		return ss.str();
    }

    Module mModule;
    Action mAction;
    string mContent;
    int64_t mLogCreateTime;
};

class Logger
{
public:
	static Logger* getInstance();

	~Logger(void);
	void sendLog(LoggerType type, Module module, Action action, const string &content);
	void recode(char* log);

private:
	Logger(void);

	static Logger* m_pInstance;
	LoggerTaskQueue *loggerTQ;
};

#endif // !__LOGGER_H__

