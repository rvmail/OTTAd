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

mutex initialize_mutex;
mutex exit_mutex;

static int isCurlInit = 0;

string ICNTV_Login_deviceLogin(string path)
{
    initialize_mutex.lock();

    LOG(DEBUG) << "ICNTV_Login_deviceLogin...";
    LOG(DEBUG) << "path=" << path;

#ifdef GIT_VERSION
    LOG(DEBUG) << "sdk commit version on git: " << GIT_VERSION;
#endif

    if (isCurlInit == 0)
    {
        LOG(DEBUG) << "curl_global_init()";
        curl_global_init(CURL_GLOBAL_DEFAULT);
        isCurlInit = 1;
    }

    string ret;
    if (Login::getInstance()->getLoginStatus() != LoginStatus::Success)
    {
        dataCache::getInstance()->setPath(path);
        ret = Login::getInstance()->startLogin();
    }

    initialize_mutex.unlock();

    return ret;
}

bool sdkExit(void)
{
    exit_mutex.lock();

    LOG(DEBUG) << "sdkExit...";

    Login::getInstance()->stopLogin();
    //Poco::Process::kill(Poco::Process::id());

    if (isCurlInit == 1)
    {
        LOG(DEBUG) << "curl_global_cleanup()";
        curl_global_cleanup();
        isCurlInit = 0;
    }

    LOG(DEBUG) << "sdkExit OK";

    exit_mutex.unlock();

    return true;
}

int getVersion(string &version)
{
    version = SDK_VERSION;
    version += "(";
    version += GIT_VERSION;
    version += ")";
    LOG(DEBUG) << "SDK_VERSION: " << version;

    return 0;
}

int getDeviceID(string &deviceID)
{
    if (Login::getInstance()->getLoginStatus() != LoginStatus::Success)
    {
        LOG(ERROR) << "login status is not success";
        return -1;
    }

    deviceID = Login::getInstance()->getDeviceID();
    LOG(DEBUG) << "deviceID=" << deviceID;

    return 0;
}

int getUserID(string &userID)
{
    if (Login::getInstance()->getLoginStatus() != LoginStatus::Success)
    {
        LOG(ERROR) << "login status is not success";
        return -1;
    }

    userID = Login::getInstance()->getUserID();
    LOG(DEBUG) << "userID=" << userID;

    return 0;
}

int getTemplateID(string &templateID)
{
    if (Login::getInstance()->getLoginStatus() != LoginStatus::Success)
    {
        LOG(ERROR) << "login status is not success";
        return -1;
    }

    templateID = Login::getInstance()->getTemplateID();
    LOG(DEBUG) << "templateID=" << templateID;

    return 0;
}

int getPlatformID(string &platformID)
{
    if (Login::getInstance()->getLoginStatus() != LoginStatus::Success)
    {
        LOG(ERROR) << "login status is not success";
        return -1;
    }

    platformID = Login::getInstance()->getPlatformID();
    LOG(DEBUG) << "platformID=" << platformID;

    return 0;
}

int getServerAddress(string type, string &serverAddr)
{
    if (Login::getInstance()->getLoginStatus() != LoginStatus::Success)
    {
        LOG(ERROR) << "login status is not success";
        return -1;
    }

    serverAddr = Login::getInstance()->getServerAddress(type);
    LOG(DEBUG) << type << " serverAddr=" << serverAddr;

    return 0;

}

int setNetworkType(int type)
{
    setNetType(type);

    return 0;
}


