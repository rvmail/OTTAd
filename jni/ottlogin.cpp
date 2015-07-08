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

#include <Poco/Process.h>
#include "thirdparty/curl/include/curl/curl.h"

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
        LOG(DEBUG) << "curl_global_init()";
        curl_global_init(CURL_GLOBAL_DEFAULT);
        Login::getInstance()->startCheckToken();
        isInit = 1;
    }

    LOG(DEBUG) << "path=" << path;
    dataCache::getInstance()->setPath(path);
    Login::getInstance()->getLoginType();

    init_mutex.unlock();

    return true;
}

string ICNTV_Login_deviceLogin(void)
{
    login_mutex.lock();

    LOG(DEBUG) << "ICNTV_Login_deviceLogin...";

    string ret("111");
    if (Login::getInstance()->getLoginStatus() != LoginSuccess)
    {
        ret = Login::getInstance()->startLogin();
    }

    login_mutex.unlock();

    return ret;
}

string ICNTV_Login_getLoginStatus(void)
{
    return (Login::getInstance()->getLoginState());
}

bool ICNTV_Login_sdkExit(void)
{
    exit_mutex.lock();

    LOG(DEBUG) << "sdkExit...";

    Login::getInstance()->stopLogin();
    //Poco::Process::kill(Poco::Process::id());

    if (isInit == 1)
    {
        LOG(DEBUG) << "curl_global_cleanup()";
        curl_global_cleanup();
        isInit = 0;
    }

    LOG(DEBUG) << "sdkExit OK";

    exit_mutex.unlock();

    return true;
}

int ICNTV_Login_getVersion(string &version)
{
    version = SDK_VERSION;
    version += "(";
    version += GIT_VERSION;
    version += ")";
    LOG(DEBUG) << "SDK_VERSION: " << version;

    return 0;
}

int ICNTV_Login_getDeviceID(string &deviceID)
{
    if (Login::getInstance()->getLoginStatus() != LoginSuccess)
    {
        LOG(ERROR) << "login status is not success";
        return -1;
    }

    deviceID = Login::getInstance()->getDeviceID();
    LOG(DEBUG) << "deviceID=" << deviceID;

    return 0;
}

int ICNTV_Login_getUserID(string &userID)
{
    if (Login::getInstance()->getLoginStatus() != LoginSuccess)
    {
        LOG(ERROR) << "login status is not success";
        return -1;
    }

    userID = Login::getInstance()->getUserID();
    LOG(DEBUG) << "userID=" << userID;

    return 0;
}

int ICNTV_Login_getTemplateID(string &templateID)
{
    if (Login::getInstance()->getLoginStatus() != LoginSuccess)
    {
        LOG(ERROR) << "login status is not success";
        return -1;
    }

    templateID = Login::getInstance()->getTemplateID();
    LOG(DEBUG) << "templateID=" << templateID;

    return 0;
}

int ICNTV_Login_getPlatformID(string &platformID)
{
    if (Login::getInstance()->getLoginStatus() != LoginSuccess)
    {
        LOG(ERROR) << "login status is not success";
        return -1;
    }

    platformID = Login::getInstance()->getPlatformID();
    LOG(DEBUG) << "platformID=" << platformID;

    return 0;
}

int ICNTV_Login_getServerAddress(string type, string &serverAddr)
{
    if (Login::getInstance()->getLoginStatus() != LoginSuccess)
    {
        LOG(ERROR) << "login status is not success";
        return -1;
    }

    serverAddr = Login::getInstance()->getServerAddress(type);
    LOG(DEBUG) << type << " serverAddr=" << serverAddr;

    return 0;

}

int ICNTV_Login_setNetworkType(int type)
{
    setNetType(type);

    return 0;
}


