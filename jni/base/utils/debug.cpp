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
*   @author   zhang.mingliang@icntv.tv
*   @data     20150713
*/

#include "debug.h"
#include "base/configure/icntvConfigure.h"
#include "basefile.h"
#include "DeviceInfo.h"
#include "SystemClock.h"
#include "base/dataCache.h"
#include "base/utils/log.h"

#include <string.h>
#include <sys/timeb.h>
#include <unistd.h>
#include <string>

LogOperate* LogOperate::m_instance = NULL;

LogOperate* LogOperate::getInstance()
{
    if (m_instance == NULL)
    {
        m_instance = new LogOperate;
    }

    return m_instance;
}

LogOperate::LogOperate(): m_defaultLogLevel(0xFF),
                          m_logOutput(0xFF)
{

}
LogOperate::~LogOperate()
{

}

void LogOperate::getConfig(void)
{
    int debug = icntvConfigure::getInstance()->getIntValue("LOG", \
                      "DEBUG", "/ini/DeviceInfo.ini");
    int info = icntvConfigure::getInstance()->getIntValue("LOG", \
                      "INFO", "/ini/DeviceInfo.ini");
    int warn = icntvConfigure::getInstance()->getIntValue("LOG", \
                      "WARN", "/ini/DeviceInfo.ini");
    int error  = icntvConfigure::getInstance()->getIntValue("LOG", \
                      "ERROR", "/ini/DeviceInfo.ini");

    if (debug != 0)
    {
        m_defaultLogLevel |= LOG_LEVEL_DEBUG;
    }

    if (info != 0)
    {
        m_defaultLogLevel |= LOG_LEVEL_INFO;
    }

    if (warn != 0)
    {
        m_defaultLogLevel |= LOG_LEVEL_WARN;
    }

    if (error != 0)
    {
        m_defaultLogLevel |= LOG_LEVEL_ERROR;
    }

    int loglevel = icntvConfigure::getInstance()->getIntValue("LOG", \
                                "LogOpen", "/ini/DeviceInfo.ini");
    if (loglevel == 0)
    {
        m_defaultLogLevel = 0x00;
    }

    //LOG(DEBUG) << "m_defaultLogLevel=" << m_defaultLogLevel;

//    std::string mac = getMac(1);
//    if (mac.empty())
//    {
//        mac = getMac(0);
//    }
//
//    if (!mac.empty())
//    {
//        //MAC 12:23:34:45:56:67 --> 122334455667
//        std::string::iterator it;
//        for (it =mac.begin(); it != mac.end(); it++)
//        {
//            if (*it == ':')
//            {
//                mac.erase(it);
//            }
//        }
//    }
//
//    SystemClock t;
//    std::string time = t.getTime();

    std::string path = dataCache::getInstance()->getPath();
    m_logFileName = path + "/ini/runtime.log";

    BaseFile f;
    if (f.isExist(m_logFileName.c_str()))
    {
        f.fileRemove(m_logFileName.c_str());
    }
}

void LogOperate::logInit()
{
    static int isInit = 0;

    if (isInit == 0)
    {
        getConfig();
        isInit = 1;
    }
}

void LogOperate::logOutput(eLogLevel logLevel, const char *format, ...)
{
    char buffer[MAX_LOG_MSG_SIZE];
    va_list ap;

    if (logLevel & m_defaultLogLevel)
    {
        va_start(ap, format);
        vsnprintf(buffer, sizeof(buffer) - 1, format, ap);
        va_end(ap);

        if (LOG_OUTPUT_SCR & m_logOutput)
        {
            if (logLevel == LOG_LEVEL_DEBUG)
            {
                LOGD("%s", buffer);
            }
            else if (logLevel == LOG_LEVEL_INFO)
            {
                LOGI("%s", buffer);
            }
            else if (logLevel == LOG_LEVEL_WARN)
            {
                LOGW("%s", buffer);
            }
            else if (logLevel == LOG_LEVEL_ERROR)
            {
                LOGE("%s", buffer);
            }
            else
            {
                //Do nothing
            }

        }

        if (LOG_OUTPUT_FILE & m_logOutput)
        {
            if (logLevel == LOG_LEVEL_DEBUG)
            {
                return;    //do not write to file if LOG_LEVEL_DEBUG
            }

            if (m_logFileName.empty())
            {
                return;
            }

            BaseFile bfile;
            if (!bfile.fileOpen(m_logFileName.c_str(), "a+"))
            {
                return;
            }

            if (bfile.getSize() > MAX_LOG_FILE_SIZE)
            {
                bfile.fileClose();
                bfile.fileRemove(m_logFileName.c_str());
                bfile.fileOpen(m_logFileName.c_str(), "a+");
            }

            bfile.fileSeek(0, SEEK_END);
            bfile.fileWrite(buffer, strlen(buffer));
            bfile.fileClose();
        }
    }
}

std::string LogOperate::getLogFileName()
{
    return m_logFileName;
}




