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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <unistd.h>

#include "inifile.h"
#include "icntvConfigure.h"
#include "base/dataCache.h"
#include "common.h"
#include "debug.h"

using namespace std;

const char LIBS_FILE_PATH[]           = "/libs/armeabi-v7a";

//DeviceInfo.ini
const char DEVICE_CONFIG_FILE[]       ="/ini/DeviceInfo.ini";
const char DEVICE_SECTION[]           = "DEVICE";
const char LOGINSERVER_KEY[]          = "LoginServer";
const char PLATFORMID_KEY[]           = "PlatformId";
const char LOGINTYPE_KEY[]            = "LoginType";

//DeviceID.ini
const char DEVICEID_CONFIG_FILE[]     = "/ini/DeviceID.ini";
const char DEVICEID_SECTION[]         = "DEVICE";
const char DEVICEID_KEY[]             = "DeviceID";

icntvConfigure* icntvConfigure::m_pInstance = NULL;

icntvConfigure* icntvConfigure::getInstance()
{
    if( m_pInstance == NULL)
        m_pInstance = new icntvConfigure();

    return m_pInstance;
}

icntvConfigure::icntvConfigure()
{

}

icntvConfigure::~icntvConfigure()
{

}

int icntvConfigure::getLoginServer(char *serverAddr, int size)
{
    return getStrValue(DEVICE_SECTION, LOGINSERVER_KEY, serverAddr, \
                       size, DEVICE_CONFIG_FILE);
}

int icntvConfigure::getDeviceID(char *id, int size)
{
    return getStrValue(DEVICEID_SECTION, DEVICEID_KEY, id, size, DEVICEID_CONFIG_FILE);
}

int icntvConfigure::setDeviceID(const char *id)
{
    return setKeyValue(DEVICEID_SECTION, DEVICEID_KEY, id, DEVICEID_CONFIG_FILE);
}

int icntvConfigure::getPlatformID(char *id, int size)
{
    return getStrValue(DEVICE_SECTION, PLATFORMID_KEY, id, size, DEVICE_CONFIG_FILE);
}

int icntvConfigure::getLoginType(char *type, int size)
{
    return getStrValue(DEVICE_SECTION, LOGINTYPE_KEY, type, size, DEVICE_CONFIG_FILE);
}

int icntvConfigure::setLoginType(const char *type)
{
    return setKeyValue(DEVICE_SECTION, LOGINTYPE_KEY, type, DEVICE_CONFIG_FILE);
}

int icntvConfigure::getFilePath(char *szFile, const char *filePath /* = NULL */)
{
    if (szFile == NULL)
    {
        return -1;
    }

    char szEnv[NUM_256] = {0};
    char *path = NULL;

#ifdef LOCAL_SERVER
    // compile to local sever, config file add to binary file path
    getcwd(szEnv, sizeof(szEnv));

#elif PURE_SDK
    // compile to sdk, cdonfig file add to ../ini
    if (dataCache::getInstance()->getPath(szEnv) == -1)
    {
        strcpy(szEnv, "/system/etc");
        LOGDEBUG("pure sdk get path failed, default path was used: /system/etc\n");
    }

#elif ENV_SDK
    // compile to sdk, config file path is alterable if set environment value
    path = getenv("ICNTV_SDK_PATH");
    if (path != NULL)
    {
        strcpy(szEnv, path);
        LOGDEBUG("szEnv=%s\n", szEnv);
    }
    else
    {
        strcpy(szEnv, "/system/etc");
    }
#else
    getcwd(szEnv, sizeof(szEnv));

#endif

    if (filePath == NULL)
    {
        sprintf(szFile,"%s%s", szEnv, DEVICE_CONFIG_FILE);
    }
    else
    {
        sprintf(szFile,"%s%s", szEnv, filePath);
    }

    LOGDEBUG("szFile=%s\n", szFile);
    return 0;
}

int icntvConfigure::getStrValue(const char *section, const char *key , \
        char *keyvalue, const int bufsize, const char *filePath)
{
    if (filePath == NULL)
    {
        return -1;
    }

    char szFile[NUM_256] = {0};
    getFilePath(szFile, filePath);

    if(!read_profile_string(section, key, keyvalue, bufsize, "", szFile))
    {
        LOGDEBUG("Get config infor failed, [%s][%s][%s]\n", szFile, section, key);
        return -1;
    }
    else
    {
        LOGDEBUG("[%s][%s]%s=%s\n", szFile, section, key, keyvalue);
    }

    return 0;
}

int icntvConfigure::getIntValue(const char *section, const char *key, \
                                const char *filePath)
{
    int nValue = 0;
    char szFile[NUM_256] = {0};

    if (filePath == NULL)
    {
        return -1;
    }

    getFilePath(szFile, filePath);
    nValue = read_profile_int(section, key, -1, szFile);

    LOGDEBUG("[%s][%s]%s=%d", szFile, section, key, nValue);
    return nValue;
}

int icntvConfigure::setKeyValue(const char *section, const char *key , \
                                const char *keyvalue, const char *filePath)
{
    if (filePath == NULL)
    {
        return -1;
    }

    char szFile[NUM_256] = {0};
    getFilePath(szFile, filePath);

    if(!write_profile_string(section, key, keyvalue, szFile))
    {
        LOGERROR("write value[%s] to config file[%s-%s-%s] failed!!\n", \
                  keyvalue, szFile, section, key);
        return -1;
    }
    return 0;
}



