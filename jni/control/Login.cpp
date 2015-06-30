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

#include <sstream>
#include <unistd.h>

#include "Login.h"
#include "../common.h"
#include "base/network/icntvHttp.h"
#include "base/utils/log.h"
#include "base/utils/string.h"
#include "base/utils/DeviceInfo.h"

#include "base/parse/initParse.h"
#include "base/parse/loginParse.h"
#include "base/configure/icntvConfigure.h"

#define LOGIN_RETRY_COUNT      3
#define LOGIN_RETRY_WAIT_TIME  2    //second

Login* Login::m_pInstance = NULL;

Login* Login::getInstance()
{
    if (m_pInstance == NULL)
    {
        m_pInstance = new Login;
    }

    return m_pInstance;
}

Login::Login(void): mLoginStatus(LoginStatus::NotLogin)
{
    mFirstLogin = (mDeviceId.empty() || mPlatformId.empty());
}

Login::~Login(void)
{
}

LoginStatus Login::getLoginStatus()
{
    return mLoginStatus;
}

string Login::getToken()
{
    if (mLoginStatus == LoginStatus::Success)
    {
        return mToken;
    }
    return "";
}

string Login::getDeviceID()
{
    return mDeviceId;
}

string Login::getUserID()
{
    return mUserId;
}

string Login::getPlatformID()
{
    return mPlatformId;
}

string Login::getTemplateID()
{
    return mTemplateId;
}

string Login::getServerAddress(string type)
{
    for (MapServerList::iterator it = mServerList.begin(); \
            it != mServerList.end(); ++it)
    {
        if (compareCaseInsensitive(it->first, type))
        {
            return it->second.url;
        }
    }

    return "";
}

string Login::getConfigure(ConfigType type)
{
    char serverAddr[NUM_128] = {0};
    char buffer[NUM_32] = {0};

    switch (type)
    {
        case configLoginAddr:
            icntvConfigure::getInstance()->getLoginServer(serverAddr, NUM_128);
            if (serverAddr[0] == '\0')
            {
                LOG(ERROR) << "Get login server address failed, use default address";
                return  "http://tms.is.ysten.com:8080/yst-tms";
            }
            LOG(DEBUG) << "Login serverAddr : " << serverAddr;
            return string(serverAddr);
            break;
        case configDeviceId:
            icntvConfigure::getInstance()->getDeviceID(buffer, NUM_32);
            return string(buffer);
            break;
        default:
            break;
    }

    return "";
}

bool Login::setConfigure(ConfigType type, const string val)
{
    switch (type)
    {
        case configLoginAddr:
            break;
        case configDeviceId:
            return icntvConfigure::getInstance()->setDeviceID(val.c_str());
            break;
        default:
            break;
    }

    return false;
}

bool Login::isFirstLogin()
{
    return mFirstLogin;
}

int Login::doActivate()
{
    LOG(DEBUG) << "doActivate start...";

    int ret;
    icntvHttp http;
    string response;
    string host(getConfigure(configLoginAddr));
    string path("/deviceInit.action");
    stringstream query;
    query << "mac=" << getMacBySocket();

    ret = http.getData(host, path, query.str(), response);
    if (ret != 0)
    {
        LOG(ERROR) << "http.getData() error!";
        return -1;
    }

    initParse mInitParse;
    InitResponse mInitResponse;

    mInitParse.parse(response.c_str(), &mInitResponse);
    if (mInitResponse.status != 1)
    {
        LOG(ERROR) << "Response error, resultCode=" << mInitResponse.status;
        return -2;
    }

    mDeviceId = mInitResponse.deviceid;
    LOG(DEBUG) << "DeviceId: " << mDeviceId;

    setConfigure(configDeviceId, mDeviceId);

    return 0;
}

int Login::doAuthenticate()
{
    LOG(DEBUG) << "doAuthenticate start...";

    int ret;
    icntvHttp http;
    string response;
    string host(getConfigure(configLoginAddr));
    string path("/device!deviceLogin.action");
    stringstream query;
    query << "deviceId=" << mDeviceId;
    query << "&mac=" << getMacBySocket();

    ret = http.getData(host, path, query.str(), response);
    if (ret != 0)
    {
        LOG(ERROR) << "http.getData() error!";
        return -1;
    }

    loginParse mLoginParse;
    LoginResponse mLoginResponse;
    mLoginParse.parse(response.c_str(), &mLoginResponse);

    if (mLoginResponse.state.compare("000") == 0)
    {
        LOG(ERROR) << "failed, doAuthenticate state=000";
        return -2;
    }

    if (mLoginResponse.state.compare("111") != 0)
    {
        LOG(ERROR) << "failed, state=" << mLoginResponse.state;
        return -3;
    }
    else
    {
        mUserId = mLoginResponse.userId;
        mTemplateId = mLoginResponse.templateId;
        mServerList= mLoginResponse.serverList;
    }

    return 0;
}

int Login::startLogin()
{
    int ret = -1;
    int retryCount = 0;
    bool needDoActivate = false;

    LOG(DEBUG) << "startLogin...";

    mDeviceId = getConfigure(configDeviceId);
    if (mDeviceId.empty())
    {
        needDoActivate = true;
    }

    for (retryCount = 0; retryCount < LOGIN_RETRY_COUNT; retryCount++)
    {
        if (mLoginStatus == LoginStatus::ForceStop)
        {
            LOG(DEBUG) << "stopLogin has been invoked";
            return -2;
        }
        else
        {
            mLoginStatus = LoginStatus::Logging;
        }

        if (needDoActivate)
        {
            ret = doActivate();
            if (ret != 0)
            {
                sleep(LOGIN_RETRY_WAIT_TIME);
                continue;
            }

            needDoActivate = false;
            retryCount = 0;
        }

        ret = doAuthenticate();
        if (ret == 0)
        {
            LOG(DEBUG) << "doAuthenticate OK";
            break;
        }
        else if (ret == -2)
        {
            needDoActivate = true;
            sleep(LOGIN_RETRY_WAIT_TIME);
            continue;
        }
        else
        {
            sleep(LOGIN_RETRY_WAIT_TIME);
            continue;
        }
    }

    if (retryCount == LOGIN_RETRY_COUNT)
    {
        LOG(ERROR) << "login failed, up to the max retry times";
        mLoginStatus = LoginStatus::LoginFailed;
        return -1;
    }

    mLoginStatus = LoginStatus::Success;
    LOG(DEBUG) << "Login success";
    return 0;
}

void Login::stopLogin()
{
    if (mLoginStatus == LoginStatus::Logging)
    {
        mLoginStatus = LoginStatus::ForceStop;
    }
    else
    {
        mLoginStatus = LoginStatus::NotLogin;
    }
}
