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

#include "ottlogin.h"
#include "common.h"
#include "base/utils/log.h"
#include "base/utils/DeviceInfo.h"
#include "base/dataCache.h"
#include "control/Login.h"
#include "base/utils/debug.h"
#include "logupload.h"
#include "curl.h"

#include <mutex>

mutex init_mutex;
mutex login_mutex;
mutex exit_mutex;

static int isInit = 0;

bool ICNTV_Login_sdkInit(string path)
{
    init_mutex.lock();

    if (isInit == 0)
    {
        //set the path of configuration files
        dataCache::getInstance()->setPath(path);

        LogOperate::getInstance()->logInit();

        LOGINFO("ICNTV_Login_sdkInit...\n");
        LOGINFO("path=%s\n", path.c_str());

        LOGDEBUG("curl_global_init()\n");
        curl_global_init(CURL_GLOBAL_DEFAULT);

        Login::getInstance()->getLoginType();
        Login::getInstance()->startCheckToken();

        isInit = 1;
        LOGINFO("ICNTV_Login_sdkInit success\n");
    }
    else
    {
        LOGWARN("SDK is already initialized\n");
    }

    init_mutex.unlock();

    return true;
}

string ICNTV_Login_deviceLogin(void)
{
    if (isInit == 0)
    {
        LOGERROR("deviceLogin MUST after sdkInit!!!\n");
        return "889";
    }

    login_mutex.lock();

    LOGINFO("ICNTV_Login_deviceLogin...\n");

    string ret("111");
    if (Login::getInstance()->getLoginStatus() != LoginSuccess)
    {
        ret = Login::getInstance()->startLogin();
    }

    login_mutex.unlock();

    LOGINFO("ICNTV_Login_deviceLogin return %s\n", ret.c_str());

    return ret;
}

string ICNTV_Login_getLoginStatus(void)
{
    string stat = Login::getInstance()->getLoginState();
    LOGINFO("ICNTV_Login_getLoginStatus=%s\n", stat.c_str());
    return stat;
}

bool ICNTV_Login_sdkExit(void)
{
    exit_mutex.lock();

    LOGINFO("ICNTV_Login_sdkExit...\n");

    Login::getInstance()->stopLogin();
    //Poco::Process::kill(Poco::Process::id());

    if (isInit == 1)
    {
        LOGINFO("curl_global_cleanup\n");
        curl_global_cleanup();
        isInit = 0;
    }

    LOGINFO("ICNTV_Login_sdkExit OK\n");

    exit_mutex.unlock();

    return true;
}

int ICNTV_Login_getVersion(string &version)
{
    version = SDK_VERSION;
    version += "(";
    version += GIT_VERSION;
    version += ")";
    LOGINFO("SDK_VERSION: %s\n", version.c_str());

    return 0;
}

int ICNTV_Login_getDeviceID(string &deviceID)
{
    if (Login::getInstance()->getLoginStatus() != LoginSuccess)
    {
        LOGERROR("login status is not success\n");
        return -1;
    }

    deviceID = Login::getInstance()->getDeviceID();

    return 0;
}

int ICNTV_Login_getUserID(string &userID)
{
    if (Login::getInstance()->getLoginStatus() != LoginSuccess)
    {
        LOGERROR("login status is not success\n");
        return -1;
    }

    userID = Login::getInstance()->getUserID();
    LOGINFO("userID=%s\n", userID.c_str());

    return 0;
}

int ICNTV_Login_getTemplateID(string &templateID)
{
    if (Login::getInstance()->getLoginStatus() != LoginSuccess)
    {
        LOGERROR("login status is not success\n");
        return -1;
    }

    templateID = Login::getInstance()->getTemplateID();
    LOGINFO("templateID=%s\n", templateID.c_str());

    return 0;
}

int ICNTV_Login_getPlatformID(string &platformID)
{
    if (Login::getInstance()->getLoginStatus() != LoginSuccess)
    {
        LOGERROR("login status is not success\n");
        return -1;
    }

    platformID = Login::getInstance()->getPlatformID();
    LOGINFO("platformID=%s\n", platformID.c_str());

    return 0;
}

int ICNTV_Login_getServerAddress(string type, string &serverAddr)
{
    if (Login::getInstance()->getLoginStatus() != LoginSuccess)
    {
        LOGERROR("login status is not success\n");
        return -1;
    }

    serverAddr = Login::getInstance()->getServerAddress(type);
    LOGINFO("%s serverAddr=%s\n", type.c_str(), serverAddr.c_str());

    return 0;
}

int ICNTV_Login_getToken(string &token)
{
    if (Login::getInstance()->getLoginStatus() != LoginSuccess)
    {
        LOGERROR("login status is not success\n");
        return -1;
    }

    token = Login::getInstance()->getToken();
    LOGINFO("token=%s\n", token.c_str());

    return 0;
}

int ICNTV_Login_setNetworkType(int type)
{
    setNetType(type);

    return 0;
}

int ICNTV_Login_logUpload(void)
{
    LOGINFO("ICNTV_Login_logUpload..\n");
    LogUpload::getInstance()->startUpload();

    return 0;
}



