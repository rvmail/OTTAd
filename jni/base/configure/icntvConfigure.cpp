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
#ifndef WIN32
#include <unistd.h>
#endif

#include "inifile.h"
#include "icntvConfigure.h"
#include "base/utils/log.h"
#include "base/dataCache.h"
#include "common.h"

using namespace std;

const char LIBS_FILE_PATH[]           = "/libs/armeabi-v7a";

//device ID
const char DEVICEID_CONFIG_FILE[]     = "/ini/DeviceID.ini";
const char DEVICEID_SECTION[]         = "DEVICE";
const char DEVICEID_KEY[]             = "DeviceID";

const char DEVICE_CONFIG_FILE[]       ="/ini/DeviceInfo.ini";
const char DEVICE_SECTION[]           = "DEVICE";
const char LOGINSERVER_KEY[]          = "LoginServer";
const char PLATFORMID_KEY[]           = "PlatformId";

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
        LOG(DEBUG) << "pure sdk get path failed, default path was used: /system/etc";
    }

    /*char szCurrPath[NUM_256] = {0};
    char szTmp[NUM_256] = {0};
    getcwd(szCurrPath, sizeof(szCurrPath));
    LOG(DEBUG) << "szCurrPath : " << szCurrPath;
    path = strstr(szCurrPath, LIBS_FILE_PATH);
    if (path != NULL)
    {
        memcpy(szEnv, szCurrPath, strlen(szCurrPath) - strlen(path));
    }
    else
    {
        strcpy(szEnv, szCurrPath);
    }*/
#elif ENV_SDK
    // compile to sdk, config file path is alterable if set environment value
    path = getenv("ICNTV_SDK_PATH");
    if (path != NULL)
    {
        strcpy(szEnv, path);
        printf("szEnv = %s", szEnv);
    }
    else
    {
        strcpy(szEnv, "/system/etc");
    }
#else
#ifdef WIN32
    strcpy(szEnv,"./");
#elif LINUX
    getcwd(szEnv, sizeof(szEnv));
#else
    LOG(DEBUG) << "Please choose a platform";
#endif
#endif

    if (filePath == NULL)
    {
        sprintf(szFile,"%s%s", szEnv, DEVICE_CONFIG_FILE);
    }
    else
    {
        sprintf(szFile,"%s%s", szEnv, filePath);
    }

    LOG(DEBUG) << "szFile : " << szFile;
    return 0;
}

int icntvConfigure::getStrValue(const char *section, const char *key , \
        char *keyvalue, const int bufsize, const char *filePath)
{
    if (filePath == NULL)
    {
        return -1;
    }

    char szFile[NUM_256];
    memset(szFile, 0, NUM_256);

    char path[NUM_256] = {0};
    getFilePath(path, filePath);
    memcpy(szFile, path, strlen(path));

    LOG(DEBUG) << "base path: " << szFile;
    if(!read_profile_string(section, key, keyvalue, bufsize, "", szFile))
    {
        LOG(ERROR) << "Get config information failed";
        return -1;
    }
    else
    {
        LOG(DEBUG) << key << "=" << keyvalue;
    }

    return 0;
}

int icntvConfigure::getIntValue(const char *section, const char *key, \
                                const char *filePath)
{
    int nValue = 0;
    char szFile[NUM_256] = {0};
    char path[NUM_256] = {0};

    if (filePath == NULL)
    {
        return -1;
    }

    getFilePath(path, filePath);;
    memcpy(szFile, path, strlen(path));
    nValue = read_profile_int(section, key, 0, szFile);

    LOG(DEBUG) << key << "=" << nValue;
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
    char path[NUM_256] = {0};

    getFilePath(path, filePath);
    memcpy(szFile, path, strlen(path));

    if(!write_profile_string(section, key, keyvalue, szFile))
    {
        LOG(ERROR) << "write config file failed!";
        return -1;
    }
    return 0;
}



