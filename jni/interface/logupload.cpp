/**
*
* Copyright 2014 Future TV, Inc.
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

#include "logupload.h"
#include "basefile.h"
#include "icntvHttp.h"
#include "debug.h"
#include "baseThread.h"
#include "Login.h"
#include "DeviceInfo.h"
#include "zlib.h"
#include "dataCache.h"
#include "icntvConfigure.h"

#include <string>
#include <string.h>

LogUpload* LogUpload::m_instance = NULL;

LogUpload* LogUpload::getInstance()
{
    if (!m_instance)
    {
        m_instance = new LogUpload;
    }

    return m_instance;
}

LogUpload::LogUpload(void): m_isUploading(false)
{
    char buf[256] = {0};
    icntvConfigure::getInstance()->getStrValue("LOG", "LogServer", \
                    buf, sizeof(buf) - 1, "/ini/DeviceInfo.ini");
    m_serverAddr = buf;
}


LogUpload::~LogUpload(void)
{
}

int LogUpload::comress2file(const char *srcFileName, const char *dstFileName)
{
    if (srcFileName == NULL || dstFileName == NULL)
    {
        LOGERROR("param is NULL\n");
        return -1;
    }

    BaseFile f;
    if (!f.fileOpen(srcFileName, "r"))
    {
        LOGERROR("fileOpen error\n");
        return -1;
    }

    int size = f.getSize();
    char *buf = (char *)malloc(size);
    if (!buf)
    {
        LOGERROR("malloc error\n");
        f.fileClose();
        return -1;
    }

    int readSize = f.fileRead(buf, size);
    if (readSize != size)
    {
        LOGERROR("fileRead error\n");
    }

    gzFile out = gzopen(dstFileName, "wb6f");
    if(!out)
    {
        LOGERROR("gzopen error, out is NULL\n");
        free(buf);
        buf = NULL;
        f.fileClose();
        return -1;
    }

    LOGINFO("readSize=%d\n", readSize);
    if(gzwrite(out, buf, (unsigned)readSize) != readSize)
    {
        LOGERROR("gzwrite error\n");
    }

    gzclose(out);
    free(buf);
    buf = NULL;
    f.fileClose();

    return 0;
}

void *LogUpload::upload(void *param)
{
    LogUpload *instance = (LogUpload *)param;

    int ret = instance->comress2file(instance->m_logFile.c_str(), \
                                     instance->m_compressLogFile.c_str());
    if (ret != 0)
    {
        return NULL;
    }

    BaseFile f;
    if (!f.fileOpen(instance->m_compressLogFile.c_str(), "r"))
    {
        return NULL;
    }

    int size = f.getSize();
    char *data = (char *)malloc(size);
    if (!data)
    {
        f.fileClose();
        f.fileRemove(instance->m_compressLogFile.c_str());
        return NULL;
    }

    ret = f.fileRead(data, size);
    if (ret == -1)
    {
        f.fileClose();
        f.fileRemove(instance->m_compressLogFile.c_str());
        free(data);
        data = NULL;
        return NULL;
    }
    f.fileClose();
    f.fileRemove(instance->m_compressLogFile.c_str());

    std::string mac = getMac(1);
    if (mac.empty())
    {
        mac = getMac(0);
    }

    if (mac.empty())
    {
        LOGERROR("Not get MAC\n");
        free(data);
        data = NULL;
        return NULL;
    }

    char buf[128] = {0};
    //MAC(string) --> num(long long)
    long long macnum = convertMac2Num(mac);
    snprintf(buf, sizeof(buf) - 1, "%lld", macnum);
    std::string id(buf);

    snprintf(buf, sizeof(buf) - 1, "%lld", macnum ^ 10000L);
    std::string uk(buf);

    //LOGDEBUG("id=%s, uk=%s\n", id.c_str(), uk.c_str());

    icntvHttp http;
    http.setHeader("ID", id);
    http.setHeader("UK", uk);

    std::string host(instance->m_serverAddr);
    std::string path("/app/receive");
    std::string response;
    ret = http.postData(host, path, data, size, response);
    if (ret != 0)
    {
        free(data);
        data = NULL;
        return NULL;
    }

    free(data);
    data = NULL;

    if (response.compare("success") != 0)
    {
        return NULL;
    }

    return 0;
}

int LogUpload::startUpload()
{
    if (m_serverAddr.empty())
    {
        LOGERROR("m_serverAddr is empty\n");
        return -1;
    }

    m_logFile = LogOperate::getInstance()->getLogFileName();
    if (m_logFile.empty())
    {
        LOGERROR("m_logFile is empty\n");
        return -1;
    }

    m_compressLogFile = dataCache::getInstance()->getPath() + "/ini/log.gz";

    baseThread thread;
    thread.startThread(LogUpload::upload, LogUpload::getInstance());

    return 0;
}


