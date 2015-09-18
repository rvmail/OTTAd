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
#include "base/utils/misc.h"
#include "base/utils/DeviceInfo.h"

#include "base/parse/initParse.h"
#include "base/parse/loginParse.h"
#include "base/parse/tokenParse.h"
#include "base/configure/icntvConfigure.h"
#include "base/baseThread.h"
#include "debug.h"
#include "icntvEncrypt.h"

#define LOGIN_RETRY_COUNT      6
#define LOGIN_RETRY_WAIT_TIME  1    //second
#define CHECK_TOKEN_TIME_INTERVAL   (60 * 5)    //five minutes

#define ERR_NO                                     "0"
#define ERR_DEFALT                                 "-1"

#define ERR_LOGIN_FORCE_STOP                       "119"
#define ERR_LOGIN_NOT_INIT                         "889"

#define ERR_CHECK_TOKEN                            "260"

#define ERR_MALLOC                                 "888"

#define ERR_READ_MAC                               "755"
#define ERR_WRITE_DEVICE_ID                        "756"
#define ERR_READ_CONFIG                            "757"

#define ERR_ACTIVATE_CONNECT_TMS                   "765"
#define ERR_AUTH_CONNECT_TMS                       "766"

#define ERR_AUTH_STATE_NULL                        "774"
#define ERR_ACTIVATE_DEVICE_NULL                   "775"

#define ERR_ACTIVATE_PARSE_RESPONSE                "776"
#define ERR_AUTH_PARSE_RESPONSE                    "777"

#define ERR_CONNECT_EPG                            "788"
#define ERR_CHECKURL                               "799"


#define AES_KEY               "36b9c7e8695468dc"
#define ENCRYPT_VERSION       "1.0"
#define VERSION_INFO          "4.4.6.19"
#define VERSION_ID            "2"

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
                    m_isInit(false),
                    m_isCheckTokenStart(false),
                    m_loginType(1),
                    m_loginState(""),
                    m_loginType1ActiErrCode("444"),
                    m_loginType2ActiErrCode("444"),
                    m_loginType3ActiErrCode("444")
{
}

Login::~Login(void)
{
}

void Login::init(void)
{
    if (!m_isInit)
    {
        startCheckToken();
        getPlatformID();
        m_isInit = true;
    }
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
                LOGERROR("Get login server address failed, use default address\n");
                return  "http://tms.is.ysten.com:8080/yst-tms";
            }
            LOGINFO("Login serverAddr=%s\n", buffer);
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

        case configMacFile:
            icntvConfigure::getInstance()->getStrValue("DEVICE", "MacFile", \
                      buffer, sizeof(buffer) - 1, "/ini/DeviceInfo.ini");
            return string(buffer);
            break;

        default:
            break;
    }

    return "";
}

int Login::setConfigure(ConfigType type, const string val)
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
    else if (loginType.compare("2") == 0)
    {
        m_loginType = 2;
    }
    else if (loginType.compare("3") == 0)
    {
        m_loginType = 3;
    }
    else    //read configuration file error, 1 default
    {
        m_loginType = 1;
    }

    m_macFile = getConfigure(configMacFile);
    LOGINFO("loginType(%d), macFile(%s)\n", m_loginType, m_macFile.c_str());
}

void Login::setLoginType(void)
{
    if (m_loginType == 1)
    {
        setConfigure(configLoginType, "1");
    }
    else if (m_loginType == 2)
    {
        setConfigure(configLoginType, "2");
    }
    else if (m_loginType == 3)
    {
        setConfigure(configLoginType, "3");
    }
}

void Login::changeLoginType(void)
{
    if (m_loginType == 1)
    {
        LOGINFO("changeLoginType, 1-->2\n");
        m_loginType = 2;
    }
    else if (m_loginType == 2)
    {
        LOGINFO("changeLoginType, 2-->3\n");
        m_loginType = 3;
    }
    else if (m_loginType == 3)
    {
        LOGINFO("changeLoginType, 3-->1\n");
        m_loginType = 1;
    }
}

void Login::setActivateErrCode(string err)
{
    if (m_loginType == 1)
    {
        m_loginType1ActiErrCode = err;
    }
    else if (m_loginType == 2)
    {
        m_loginType2ActiErrCode = err;
    }
    else if (m_loginType == 3)
    {
        m_loginType3ActiErrCode = err;
    }
    else
    {

    }
}

string Login::buildQuery(eLoginType type, string mac)
{
    stringstream t;
    t << time(NULL);

    string time = t.str();

    if (!m_serverTime.empty())
    {
        time = m_serverTime;
    }

    //generate the k parameter in query
    string k;
    if (type == LoginActivate)
    {
        k = time + "/" + mac + "/" + "000000000000000" + "/" + "sn" + "/";
    }
    else if (type == LoginAuth)
    {
        k = time + "/" + mac + "/" + mDeviceId + "/" + "sn" + "/";
    }
    else
    {
        LOGERROR("type is invalid\n");
        return "";
    }

    LOGDEBUG("k=%s\n", k.c_str());

    icntvEncrypt enc;
    string aes_k = enc.aesEncrypt(k, AES_KEY);
    LOGDEBUG("aes_k=%s\n", aes_k.c_str());

    //generate the s parameter in query
    string s;
    s = aes_k + mPlatformId + ENCRYPT_VERSION + AES_KEY + time;
    LOGDEBUG("s=%s\n", s.c_str());

    string md5_s = enc.md5Encrypt(s);
    LOGDEBUG("md5_s=%s\n", md5_s.c_str());

    stringstream query;
    query << "k=" << aes_k;
    query << "&s=" << md5_s;
    query << "&v=" << ENCRYPT_VERSION;
    query << "&type=";
    query << "&platformid=" << mPlatformId;
    query << "&versioninfo=" << VERSION_INFO;
    query << "&versionid=" << VERSION_ID;

    m_serverTime.clear();
    return query.str();
}

string Login::doActivate()
{
    LOGINFO("doActivate start...\n");

    int ret;
    icntvHttp http;
    string response;
    string host(getConfigure(configLoginAddr));
    //string path("/deviceInit.action");
    string path("/init.action");

    string mac = getMac(m_loginType, m_macFile);
    if (mac.empty())
    {
        setActivateErrCode(ERR_READ_MAC);
        LOGERROR("doActivate MAC(%d) is empty\n", m_loginType);
        changeLoginType();
        return ERR_READ_MAC;
    }

    LOGINFO("[doActivate] MAC(%d)=%s\n", m_loginType, mac.c_str());

    //stringstream query;
    //query << "mac=" << mac;

    string query = buildQuery(LoginActivate, mac);

    ret = http.getData(host, path, query, response);
    if (ret != 0)
    {
        setActivateErrCode(ERR_ACTIVATE_CONNECT_TMS);
        LOGERROR("doActivate http.getData() error!\n");
        return ERR_ACTIVATE_CONNECT_TMS;
    }

    initParse mInitParse;
    InitResponse mInitResponse;

    ret = mInitParse.parse(response.c_str(), &mInitResponse);
    if (ret != 0)
    {
        setActivateErrCode(ERR_ACTIVATE_PARSE_RESPONSE);
        LOGERROR("mInitParse error\n");
        return ERR_ACTIVATE_PARSE_RESPONSE;
    }

    if (mInitResponse.resultCode != 1)
    {
        LOGERROR("resultCode=%d\n", mInitResponse.resultCode);
        //return ERR_ACTIVATE_DEVICE_NULL;
    }

    mDeviceId = mInitResponse.deviceid;
    LOGINFO("DeviceId=%s\n", mDeviceId.c_str());

    if (mDeviceId.empty())
    {
        setActivateErrCode(ERR_ACTIVATE_DEVICE_NULL);
        LOGERROR("mDeviceId is empty\n");

        if (mInitResponse.state.compare("93") == 0)
        {
            m_serverTime = mInitResponse.time;
        }
        else
        {
            changeLoginType();
        }

        return ERR_ACTIVATE_DEVICE_NULL;
    }

    setLoginType();

    //activate success, write LoginType into DeviceID.ini
    char buf[32] = {0};
    snprintf(buf, sizeof(buf), "%d", m_loginType);

    ret = icntvConfigure::getInstance()->setKeyValue("DEVICE", \
            "LoginType", buf, "/ini/DeviceID.ini");
    if (ret != 0)
    {
        setActivateErrCode(ERR_WRITE_DEVICE_ID);
        LOGERROR("write LoginType failed\n");
        return ERR_WRITE_DEVICE_ID;
    }
    LOGINFO("write LoginType success\n");

    //activate success, write DeviceID into DeviceID.ini
    ret = setConfigure(configDeviceId, mDeviceId);
    if (ret != 0)
    {
        setActivateErrCode(ERR_WRITE_DEVICE_ID);
        LOGERROR("write deviceID failed\n");
        return ERR_WRITE_DEVICE_ID;
    }
    LOGINFO("write deviceID success\n");

    LOGINFO("doActivate success, LoginType(%d)\n", m_loginType);

    return ERR_NO;
}

string Login::doAuthenticate()
{
    LOGINFO("doAuthenticate start...\n");

    int ret;
    icntvHttp http;
    string response;
    string host(getConfigure(configLoginAddr));
    //string path("/device!deviceLogin.action");
    string path("/login.action");
    //stringstream query;
    //query << "deviceId=" << mDeviceId;

    string mac = getMac(m_loginType, m_macFile);
    if (mac.empty())
    {
        LOGERROR("[doAuthenticate] MAC(%d) is empty\n", m_loginType);
        return ERR_READ_MAC;
    }

    LOGINFO("[doAuthenticate] MAC(%d)=%s\n", m_loginType, mac.c_str());

    //query << "&mac=" << mac;

    string query = buildQuery(LoginAuth, mac);

    ret = http.getData(host, path, query, response);
    if (ret != 0)
    {
        LOGERROR("doAuthenticate http.getData() error!\n");
        return ERR_AUTH_CONNECT_TMS;
    }

    loginParse mLoginParse;
    LoginResponse mLoginResponse;
    ret = mLoginParse.parse(response.c_str(), &mLoginResponse);
    if (ret != 0)
    {
        LOGERROR("mLoginParse.parse error\n");
        return ERR_AUTH_PARSE_RESPONSE;
    }

    //state is Empty
    if (mLoginResponse.state.empty())
    {
        LOGERROR("doAuthenticate state is empty\n");
        return ERR_AUTH_STATE_NULL;
    }

    //state is 93, time is not good, so use the time from server
    if (mLoginResponse.state.compare("93") == 0)
    {
        m_serverTime = mLoginResponse.time;
    }

    if (mLoginResponse.state.compare("111") == 0 \
            || mLoginResponse.state.compare("110") == 0)
    {
        mUserId = mLoginResponse.userId;
        mTemplateId = mLoginResponse.templateId;
        mServerList= mLoginResponse.serverList;
        mToken = mLoginResponse.token;
    }

    LOGINFO("doAuthenticate end, state=%s\n", mLoginResponse.state.c_str());
    return mLoginResponse.state;
}

string Login::startLogin()
{
    if (!m_isInit)
    {
        LOGERROR("startLogin failed, Not Init\n");
        m_loginState = ERR_LOGIN_NOT_INIT;
        return ERR_LOGIN_NOT_INIT;
    }

    getLoginType();

    string ret;
    int retryCount = 0;
    bool needDoActivate = false;

    LOGINFO("startLogin...\n");

    mDeviceId = getConfigure(configDeviceId);
    if (mDeviceId.empty())
    {
        LOGWARN("Can not get device ID from configuration file, Need doActivate!\n");
        needDoActivate = true;
    }
    else
    {
        //get the LoginType form DeviceID.ini, LoginType is write when
        //doActivate is successful
        m_loginType = icntvConfigure::getInstance()->getIntValue("DEVICE", \
                           "LoginType", "/ini/DeviceID.ini");
        if (m_loginType == -1)
        {
            LOGERROR("read LoginType from DeviceID.ini failed\n");
            return ERR_READ_CONFIG;
        }
        LOGINFO("m_loginType=%d\n", m_loginType);
    }

    for (retryCount = 0; retryCount < LOGIN_RETRY_COUNT; retryCount++)
    {
        if (mLoginStatus == LoginForceStop)
        {
            LOGINFO("stopLogin has been invoked\n");
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
            LOGINFO("doAuthenticate OK\n");
            break;
        }
        else
        {
            sleep(LOGIN_RETRY_WAIT_TIME);
            continue;
        }
    }

    if (retryCount == LOGIN_RETRY_COUNT)
    {
        LOGERROR("login failed, up to the max retry times\n");
        mLoginStatus = LoginStatus::LoginFailed;

        if (needDoActivate)
        {
            LOGERROR("doActivate already try all, but failed\n");
            ret = "1" + m_loginType1ActiErrCode + "-" \
                + "2" + m_loginType2ActiErrCode + "-" \
                + "3" + m_loginType3ActiErrCode;
        }
    }
    else
    {
        mLoginStatus = LoginSuccess;
        LOGINFO("Login success\n");
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
        m_loginState = "";
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
        LOGWARN("checkToken token is empty\n");
        return 0;
    }
    path += token;

    string query;
    string response;
    icntvHttp http;
    ret = http.getData(host, path, query, response);
    if (ret != 0)
    {
        LOGERROR("checkToken http.getData() error!\n");
        return -1;
    }

    tokenParse tokenParse;
    TokenResponse tokenResponse;
    ret = tokenParse.parse(response.c_str(), &tokenResponse);
    if (ret != 0)
    {
        LOGERROR("tokenParse.parse error\n");
        return -2;
    }

    if (tokenResponse.respCode != 1)
    {
        LOGERROR("token is invalid\n");
        mLoginStatus = LoginTokenErr;
        m_loginState = ERR_CHECK_TOKEN;
        mToken.clear();
        //startLogin();
    }

    return 0;
}

void *Login::checkTokenThread(void *param)
{
    LOGINFO("checkToken start!!!\n");

    Login *plogin = (Login *)param;

    while (1)
    {
        sleep(CHECK_TOKEN_TIME_INTERVAL);
        plogin->checkToken();
    }

    LOGINFO("checkToken end!!!\n");

    return 0;
}

int Login::startCheckToken()
{
    if (m_isCheckTokenStart)
    {
        LOGWARN("Check token thread has alreadty been started\n");
        return 0;
    }

    baseThread heart;
    heart.startThread(Login::checkTokenThread, Login::getInstance());

    m_isCheckTokenStart = true;
    return 0;
}


