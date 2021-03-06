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
*/

#ifndef __DEBUG_H__
#define __DEBUG_H__

#include "SystemClock.h"

#include <android/log.h>

typedef enum
{
    LOG_LEVEL_NONE  = 0x00,
    LOG_LEVEL_DEBUG = 0x01,
    LOG_LEVEL_INFO  = 0x02,
    LOG_LEVEL_WARN  = 0x04,
    LOG_LEVEL_ERROR = 0x08,
    LOG_LEVEL_ALL   = 0xFF
}eLogLevel;

#define LOG_OUTPUT_SCR    0x01
#define LOG_OUTPUT_FILE   0x02
#define LOG_OUTPUT_SERVER 0x04

#define LOG_TAG "ottlogin"

#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

#define MAX_LOG_MSG_SIZE  (1024 * 8)  //8KB
#define MAX_LOG_FILE_SIZE  (1024 * 500)  //500KB

class LogOperate
{
public :
    ~LogOperate(void);
    static LogOperate *getInstance();

    void logInit(void);
    void logOutput(eLogLevel logLevel, const char *format, ...);
    std::string getLogFileName(void);

private:
    LogOperate(void);
    void getConfig(void);

    static LogOperate *m_instance;

    int m_defaultLogLevel;
    int m_logOutput;

    std::string m_logFileName;
};

#define LOGDEBUG(x, ...)\
{\
    LogOperate::getInstance()->logOutput(LOG_LEVEL_DEBUG, "D<%s>[%s-%d]: " x, SystemClock::getTime().c_str(), __FILE__, __LINE__,  ##__VA_ARGS__);\
}

#define LOGINFO(x, ...)\
{\
    LogOperate::getInstance()->logOutput(LOG_LEVEL_INFO, "I<%s>[%s-%d]: " x, SystemClock::getTime().c_str(), __FILE__, __LINE__,  ##__VA_ARGS__);\
}

#define LOGWARN(x, ...)\
{\
    LogOperate::getInstance()->logOutput(LOG_LEVEL_WARN, "W<%s>[%s-%d]: " x, SystemClock::getTime().c_str(), __FILE__, __LINE__,  ##__VA_ARGS__);\
}

#define LOGERROR(x, ...)\
{\
    LogOperate::getInstance()->logOutput(LOG_LEVEL_ERROR, "E<%s>[%s-%d]: " x, SystemClock::getTime().c_str(), __FILE__, __LINE__,  ##__VA_ARGS__);\
}

#endif
