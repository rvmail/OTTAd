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
#include "base/parse/tokenParse.h"
#include "base/configure/icntvConfigure.h"
#include "base/baseThread.h"

#define LOGIN_RETRY_COUNT      4
#define LOGIN_RETRY_WAIT_TIME  2    //second
#define CHECK_TOKEN_TIME_INTERVAL   (60 * 5)    //five minutes

#define ERR_NO                                     "0"
#define ERR_DEFALT                                 "-1"
#define ERR_MALLOC                                 "888"
#define ERR_READ_MAC                               "755"
#define ERR_LOGIN_FORCE_STOP                       "119"

#define ERR_ACTIVATE_CONNECT_TMS                   "765"
#define ERR_ACTIVATE_PARSE_RESPONSE                "776"
#define ERR_ACTIVATE_DEVICE_NULL                   "775"

#define ERR_AUTH_CONNECT_TMS                       "766"
#define ERR_AUTH_PARSE_RESPONSE                    "777"
#define ERR_AUTH_STATE_NULL                        "774"

#define ERR_CONNECT_EPG                            "788"
#define ERR_CHECKURL                               "799"

Login* Login::m_pInstance = NULL;

Login* Login::getInstance()
{
    if (m_pInstance == NULL)
    {
        m_pInstance = new Login;
    }

    return m_pInstance;
}

Login::Login(void): mLoginStatus(LoginNot),
                    m_isCheckTokenStart(false),
                    m_loginType(1),
                    m_loginState("000")
{
}

Login::~Login(void)
{
}

LoginStatus Login::getLoginStatus(void)
{
    return mLoginStatus;
}

string Login::getLoginState(void)
{
    return m_loginState;
}

string Login::getToken()
{
        return mToken;
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
    if (mPlatformId.empty())
    {
        mPlatformId = getConfigure(configPlatformId);
    }

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
    char buffer[NUM_128] = {0};

    switch (type)
    {
        case configLoginAddr:
            icntvConfigure::getInstance()->getLoginServer(buffer, NUM_128);
            if (buffer[0] == '\0')
            {
                LOG(ERROR) << "Get login server address failed, use default address";
                return  "http://tms.is.ysten.com:8080/yst-tms";
            }
            LOG(DEBUG) << "Login serverAddr : " << buffer;
            return string(buffer);
            break;
        case configDeviceId:
            icntvConfigure::getInstance()->getDeviceID(buffer, NUM_128);
            return string(buffer);
            break;
        case configPlatformId:
            icntvConfigure::getInstance()->getPlatformID(buffer, NUM_128);
            return string(buffer);
            break;
        case configLoginType:
            icntvConfigure::getInstance()->getLoginType(buffer, NUM_128);
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
        case configLoginType:
            return icntvConfigure::getInstance()->setLoginType(val.c_str());
            break;
        default:
            break;
    }

    return false;
}

void Login::getLoginType(void)
{
    string loginType(getConfigure(configLoginType));

    if (loginType.compare("1") == 0)
    {
        m_loginType = 1;
    }
    else if (loginType.compare("0") == 0)
    {
        m_loginType = 0;
    }
    else    //read configuration file error, 1 default
    {
        m_loginType = 1;
    }
}

void Login::setLoginType(void)
{
    if (m_loginType == 0)
    {
        setConfigure(configLoginType, "0");
    }
    else if (m_loginType == 1)
    {
        setConfigure(configLoginType, "1");
    }
}

void Login::changeLoginType(void)
{
    if (m_loginType == 0)
    {
        m_loginType = 1;
    }
    else if (m_loginType == 1)
    {
        m_loginType = 0;
    }
}

string Login::doActivate()
{
    LOG(DEBUG) << "doActivate start...";

    int ret;
    icntvHttp http;
    string response;
    string host(getConfigure(configLoginAddr));
    string path("/deviceInit.action");

    string mac = getMac(m_loginType);
    if (mac.empty())
    {
        LOG(ERROR) << "doActivate mac is empty";
        changeLoginType();
        return ERR_READ_MAC;
    }

    stringstream query;
    query << "mac=" << mac;

    ret = http.getData(host, path, query.str(), response);
    if (ret != 0)
    {
        LOG(ERROR) << "doActivate http.getData() error!";
        return ERR_ACTIVATE_CONNECT_TMS;
    }

    initParse mInitParse;
    InitResponse mInitResponse;

    ret = mInitParse.parse(response.c_str(), &mInitResponse);
    if (ret != 0)
    {
        LOG(ERROR) << "mInitParse error";
        return ERR_ACTIVATE_PARSE_RESPONSE;
    }

    if (mInitResponse.status != 1)
    {
        LOG(ERROR) << "resultCode=" << mInitResponse.status;
        //return ERR_ACTIVATE_DEVICE_NULL;
    }

    mDeviceId = mInitResponse.deviceid;
    LOG(DEBUG) << "DeviceId: " << mDeviceId;

    if (mDeviceId.empty())
    {
        LOG(ERROR) << "mDeviceId is empty";

        changeLoginType();
        return ERR_ACTIVATE_DEVICE_NULL;
    }

    setConfigure(configDeviceId, mDeviceId);
    setLoginType();

    return ERR_NO;
}

string Login::doAuthenticate()
{
    LOG(DEBUG) << "doAuthenticate start...";

    int ret;
    icntvHttp http;
    string response;
    string host(getConfigure(configLoginAddr));
    string path("/device!deviceLogin.action");
    stringstream query;
    query << "deviceId=" << mDeviceId;

    string mac = getMac(m_loginType);
    if (mac.empty())
    {
        LOG(ERROR) << "doAuthenticate mac is empty";
        return ERR_READ_MAC;
    }

    query << "&mac=" << mac;

    ret = http.getData(host, path, query.str(), response);
    if (ret != 0)
    {
        LOG(ERROR) << "doAuthenticate http.getData() error!";
        return ERR_AUTH_CONNECT_TMS;
    }

    loginParse mLoginParse;
    LoginResponse mLoginResponse;
    ret = mLoginParse.parse(response.c_str(), &mLoginResponse);
    if (ret != 0)
    {
        LOG(ERROR) << "mLoginParse.parse error";
        return ERR_AUTH_PARSE_RESPONSE;
    }

    if (mLoginResponse.state.empty())
    {
        LOG(ERROR) << "doAuthenticate state is empty";
        return ERR_AUTH_STATE_NULL;
    }

    if (mLoginResponse.state.compare("111") == 0 \
            || mLoginResponse.state.compare("110") == 0)
    {
        mUserId = mLoginResponse.userId;
        mTemplateId = mLoginResponse.templateId;
        mServerList= mLoginResponse.serverList;
        mToken = mLoginResponse.token;
    }

    return mLoginResponse.state;
}

string Login::startLogin()
{
    string ret;
    int retryCount = 0;
    bool needDoActivate = false;

    LOG(DEBUG) << "startLogin...";

    mDeviceId = getConfigure(configDeviceId);
    if (mDeviceId.empty())
    {
        LOG(DEBUG) << "Can not get device ID from configuration file, Need doActivate!";
        needDoActivate = true;
    }

    for (retryCount = 0; retryCount < LOGIN_RETRY_COUNT; retryCount++)
    {
        if (mLoginStatus == LoginForceStop)
        {
            LOG(DEBUG) << "stopLogin has been invoked";
            return ERR_LOGIN_FORCE_STOP;
        }
        else
        {
            mLoginStatus = Logining;
        }

        if (needDoActivate)
        {
            ret = doActivate();
            if (ret.compare(ERR_NO) != 0)
            {
                sleep(LOGIN_RETRY_WAIT_TIME);
                continue;
            }

            needDoActivate = false;
            retryCount = 0;
        }

        ret = doAuthenticate();
        if (ret.compare("111") == 0 || ret.compare("110") == 0)
        {
            LOG(DEBUG) << "doAuthenticate OK";
            break;
        }
        else if (ret.compare("000") == 0)
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
    }
    else
    {
        mLoginStatus = LoginSuccess;
        LOG(DEBUG) << "Login success";
    }

    m_loginState = ret;
    return ret;
}

void Login::stopLogin()
{
    if (mLoginStatus == Logining)
    {
        mLoginStatus = LoginForceStop;
    }
    else
    {
        mLoginStatus = LoginNot;
        m_loginState = "000";
    }
}

int Login::checkToken()
{
    int ret;
    string host = getServerAddress("AUTH");
    string path("/auth/checkToken/");

    string token = getToken();
    if (token.empty())
    {
        LOG(ERROR) << "checkToken token is empty";
        return 0;
    }
    path += token;

    string query;
    string response;
    icntvHttp http;
    ret = http.getData(host, path, query, response);
    if (ret != 0)
    {
        LOG(ERROR) << "checkToken http.getData() error!";
        return -1;
    }

    tokenParse tokenParse;
    TokenResponse tokenResponse;
    ret = tokenParse.parse(response.c_str(), &tokenResponse);
    if (ret != 0)
    {
        LOG(ERROR) << "tokenParse.parse error";
        return -2;
    }

    if (tokenResponse.respCode != 1)
    {
        mLoginStatus = LoginTokenErr;
        startLogin();
    }

    return 0;
}

void *Login::checkTokenThread(void *param)
{
    LOG(DEBUG) << "checkToken start!!!";

    Login *plogin = (Login *)param;

    while (1)
    {
        sleep(CHECK_TOKEN_TIME_INTERVAL);
        plogin->checkToken();
    }

    LOG(DEBUG) << "checkToken end!!!";

    return 0;
}

int Login::startCheckToken()
{
    if (m_isCheckTokenStart)
    {
        return 0;
    }

    baseThread heart;
    heart.startThread(Login::checkTokenThread, Login::getInstance());

    m_isCheckTokenStart = true;
    return 0;
}


