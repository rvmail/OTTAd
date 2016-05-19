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
#include "common.h"
#include "base/network/icntvHttp.h"
#include "base/utils/misc.h"
#include "base/utils/DeviceInfo.h"
#include "SystemClock.h"
#include "JsonParse.h"

#include "base/configure/icntvConfigure.h"
#include "base/baseThread.h"
#include "debug.h"
#include "icntvEncrypt.h"
#include "dataCache.h"

#define BOOT_TRY_TIMES         3
#define LOGIN_TRY_TIMES        4
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

#define PUB_ACTI_KEY           "36b9c7e8695468dc"

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
                    m_isCheckTokenThreadRunning(false),
                    m_loginType(1),
                    m_loginState(""),
                    m_loginType1ActiErrCode("444"),
                    m_loginType2ActiErrCode("444"),
                    m_loginType3ActiErrCode("444"),
                    m_backupServerIsUsed(false),
                    m_tmsAddress(""), m_tmsAddressBackup("")
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

string Login::getAddressFromList(string name, MapServerList &list)
{
    for (MapServerList::iterator it = list.begin(); \
                it != list.end(); ++it)
    {
        if (compareCaseInsensitive(it->first, name))
        {
            return it->second.url;
        }
    }

    return "";
}

string Login::getServerAddress(string name)
{
    return getAddressFromList(name, mServerList);
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
                return  "http://tms.ottcn.com:8080/yst-tms";
            }
            LOGINFO("Login serverAddr=%s\n", buffer);
            return string(buffer);
            break;

        case configLoginAddrBackup:
            icntvConfigure::getInstance()->getStrValue("DEVICE", "LoginServerBackup", \
                                  buffer, sizeof(buffer) - 1, "/ini/DeviceInfo.ini");
            if (buffer[0] == '\0')
            {
                LOGERROR("Get LoginServerBackup failed, use default address[125.39.27.155]\n");
                return  "http://125.39.27.155:8080/yst-tms";
            }

            LOGINFO("Login serverAddrBackup=%s\n", buffer);
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

int Login::getLoginServerAddr()
{
    m_loginServer = getConfigure(configLoginAddr);
    m_loginServerBackup = getConfigure(configLoginAddrBackup);

    //目前激活认证备用IP地址和boot接口备用IP地址相同
    m_tmsAddressBackup = m_loginServerBackup;

    m_publicActivateAddr = icntvConfigure::getInstance()->getStrValue("DEVICE", \
            "PublicActivateAddr", "/ini/DeviceInfo.ini");

    return 0;
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

    m_activateErrCode = err;
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

void Login::changeLoginServerAddr(int errorCode)
{
    if (errorCode > 0)
    {
        if (m_backupServerIsUsed)
        {
            m_backupServerIsUsed = false;
            LOGINFO("changeLoginServerAddr, backup --> default\n");
        }
        else
        {
            m_backupServerIsUsed = true;
            LOGINFO("changeLoginServerAddr, default --> backup\n");
        }
    }
}

bool Login::doBoot()
{
    LOGINFO("###doBoot start...\n");

    int ret;
    icntvHttp http;
    string response;
    string host = m_loginServer;
    string path("/boot");

    string deviceid = getConfigure(configDeviceId);

    string query("deviceId=");
    query += deviceid;

    if (m_backupServerIsUsed)
    {
        host = m_loginServerBackup;
    }

    LOGINFO("doBoot host=%s\n", host.c_str());
    ret = http.getData(host, path, query, response);
    if (ret != 0)
    {
        changeLoginServerAddr(ret);
        LOGERROR("doBoot http.getData() error!\n");
        return false;
    }

    BootResponse bootResp;
    ret = XMLParse::bootParse(response.c_str(), bootResp);
    if (ret != 0)
    {
        LOGERROR("XMLParse::bootParse error\n");
        return false;
    }

    if (bootResp.addressList.size() < 1)
    {
        LOGERROR("bootResp.addressList.size()=%d\n", bootResp.addressList.size());
        return false;
    }

    string tmsAddress = getAddressFromList("TMS", bootResp.addressList);
    if (tmsAddress == "")
    {
        LOGERROR("tmsAddress is empty\n");
        return false;
    }

    m_tmsAddress = tmsAddress;
    LOGINFO("m_tmsAddress: %s\n", m_tmsAddress.c_str());

    LOGINFO("doBoot success!!!\n");

    return true;
}

string Login::doActivate()
{
    LOGINFO("###doActivate start...\n");

    int ret;
    icntvHttp http;
    string response;
    string host = m_tmsAddress;
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

    string query = buildQuery(LoginActivate, mac);

    if (m_backupServerIsUsed)
    {
        host = m_tmsAddressBackup;
    }

    if (host == "")
    {
        LOGINFO("host(m_tmsAddress) is empty, m_tmsAddressBackup is used\n");
        host = m_tmsAddressBackup;
    }

    LOGINFO("doActivate host=%s\n", host.c_str());
    ret = http.getData(host, path, query, response);
    if (ret != 0)
    {
        changeLoginServerAddr(ret);
        setActivateErrCode(ERR_ACTIVATE_CONNECT_TMS);
        LOGERROR("doActivate http.getData() error!\n");
        return ERR_ACTIVATE_CONNECT_TMS;
    }

    InitResponse mInitResponse;

    ret = XMLParse::initParse(response.c_str(), &mInitResponse);
    if (ret != 0)
    {
        setActivateErrCode(ERR_ACTIVATE_PARSE_RESPONSE);
        LOGERROR("XMLParse::initParse error\n");
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

string Login::genePubActiToken(string mac, string apptype, string timestamp)
{
    icntvEncrypt enc;
    string temp = mac + "&" + apptype;
    string k = enc.aesEncrypt(temp, PUB_ACTI_KEY);
    LOGDEBUG("k=%s\n", k.c_str());
    string usedkey = k.substr(0, 16);
    LOGDEBUG("usedkey=%s\n", usedkey.c_str());

    string token = enc.aesEncrypt(timestamp, usedkey);
    LOGDEBUG("genePubActiToken return %s\n", token.c_str());

    return token;
}

string Login::publicActivate()
{
    LOGINFO("###publicActivate start...\n");

    int ret;
    icntvHttp http;
    string response;
    string host = m_publicActivateAddr;
    string path("/aas-api/activate");

    string mac = getMacAndRemoveColon(m_loginType, m_macFile);
    if (mac.empty())
    {
        setActivateErrCode(ERR_READ_MAC);
        LOGERROR("publicActivate MAC(%d) is empty\n", m_loginType);
        changeLoginType();
        return ERR_READ_MAC;
    }

    LOGINFO("[publicActivate] MAC(%d)=%s\n", m_loginType, mac.c_str());

    string appType = APPTYPE;
    LOGDEBUG("apptype: %s\n", appType.c_str());

    string timestamp = SystemClock::currentTimeMs();
    string token = genePubActiToken(mac, appType, timestamp);
    string ip = getIP();

    string query;
    query = "mac=" + mac;
    query += "&timestamp=" + timestamp;
    query += "&ip=" + ip;
    query += "&appkey=" + appType;
    query += "&token=" + token;

//    if (m_backupServerIsUsed)
//    {
//        host = m_tmsAddressBackup;
//    }
//
//    if (host == "")
//    {
//        LOGINFO("host(m_tmsAddress) is empty, m_tmsAddressBackup is used\n");
//        host = m_tmsAddressBackup;
//    }

    LOGINFO("publicActivate host=%s\n", host.c_str());
    ret = http.postData(host, path, query.c_str(), query.length(), response);
    if (ret != 0)
    {
        //changeLoginServerAddr(ret);
        setActivateErrCode(ERR_ACTIVATE_CONNECT_TMS);
        LOGERROR("publicActivate http.postData() error!\n");
        return ERR_ACTIVATE_CONNECT_TMS;
    }

    ActivateResponse activateResp;

    bool success = JsonParse::parseActivate(response.c_str(), &activateResp);
    if (!success)
    {
        setActivateErrCode(ERR_ACTIVATE_PARSE_RESPONSE);
        LOGERROR("JsonParse::parseActivate error\n");
        return ERR_ACTIVATE_PARSE_RESPONSE;
    }

    if (activateResp.returnCode != "0")
    {
        LOGERROR("returnCode is %s\n", activateResp.returnCode.c_str());
        setActivateErrCode(ERR_ACTIVATE_DEVICE_NULL);
        return ERR_ACTIVATE_DEVICE_NULL;
    }

    m_appCode =  activateResp.appCode;

    mDeviceId = activateResp.icntvid;
    LOGINFO("DeviceId=%s\n", mDeviceId.c_str());

    if (mDeviceId.empty())
    {
        setActivateErrCode(ERR_ACTIVATE_DEVICE_NULL);
        LOGERROR("mDeviceId is empty\n");
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

    LOGINFO("publicActivate success, LoginType(%d)\n", m_loginType);

    return ERR_NO;
}

string Login::doAuthenticate()
{
    LOGINFO("###doAuthenticate start...\n");

    int ret;
    icntvHttp http;
    string response;
    string host = m_tmsAddress;
    string path("/login.action");

    string mac = getMacAndRemoveColon(m_loginType, m_macFile);
    if (mac.empty())
    {
        LOGERROR("[doAuthenticate] MAC(%d) is empty\n", m_loginType);
        return ERR_READ_MAC;
    }

    LOGINFO("[doAuthenticate] MAC(%d)=%s\n", m_loginType, mac.c_str());

    mac = mac + "-" + m_appCode;
    string query = buildQuery(LoginAuth, mac);

    if (m_backupServerIsUsed)
    {
        host = m_tmsAddressBackup;
    }

    if (host == "")
    {
        LOGINFO("host(m_tmsAddress) is empty, m_tmsAddressBackup is used\n");
        host = m_tmsAddressBackup;
    }

    LOGINFO("doAuthenticate host=%s\n", host.c_str());
    ret = http.getData(host, path, query, response);
    if (ret != 0)
    {
        changeLoginServerAddr(ret);
        LOGERROR("doAuthenticate http.getData() error!\n");
        return ERR_AUTH_CONNECT_TMS;
    }

    LoginResponse mLoginResponse;
    ret = XMLParse::loginParse(response.c_str(), &mLoginResponse);
    if (ret != 0)
    {
        LOGERROR("XMLParse::loginParse error\n");
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
        mServerList = mLoginResponse.serverList;
        mToken = mLoginResponse.token;
    }

    LOGINFO("doAuthenticate end, state=%s\n", mLoginResponse.state.c_str());
    return mLoginResponse.state;
}

bool Login::whetherNeedActivate()
{
    bool needDoActivate = false;

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
            LOGERROR("read LoginType from DeviceID.ini failed, Need doActivate!\n");
            needDoActivate = true;
            getLoginType();
        }
        else
        {
            LOGINFO("m_loginType=%d, in DeviceID.ini, Do not need to doActivate\n", m_loginType);
        }
    }

    return needDoActivate;
}

string Login::startLogin()
{
    LOGINFO("startLogin...\n");

    if (!m_isInit)
    {
        LOGERROR("startLogin failed, Not Init\n");
        m_loginState = ERR_LOGIN_NOT_INIT;
        return ERR_LOGIN_NOT_INIT;
    }

    //get LoginType and MacFile from DeviceInfo.ini
    getLoginType();

    //get the address of the login server
    getLoginServerAddr();

    //get the address of the TMS server, try BOOT_TRY_TIMES times
    m_backupServerIsUsed = false;
    int i;
    for (i = 0; i < BOOT_TRY_TIMES; i++)
    {
        if (doBoot())
        {
            break;
        }
    }

    //activate & login
    string ret;
    bool needDoActivate = whetherNeedActivate();
    bool needReActi = true;
    m_backupServerIsUsed = false;

    for (i = 0; i < LOGIN_TRY_TIMES; i++)
    {
        if (mLoginStatus == LoginForceStop)
        {
            LOGINFO("stopLogin has been invoked\n");
            m_loginState = ERR_LOGIN_FORCE_STOP;
            return ERR_LOGIN_FORCE_STOP;
        }
        else
        {
            mLoginStatus = Logining;
        }

        if (needDoActivate)
        {
            ret = publicActivate();
            if (ret.compare(ERR_NO) != 0)
            {
                //sleep(LOGIN_RETRY_WAIT_TIME);
                continue;
            }

            needDoActivate = false;
            i = 0;
        }

        ret = doAuthenticate();
        if (ret.compare("111") == 0 || ret.compare("110") == 0)
        {
            LOGINFO("doAuthenticate OK\n");
            break;
        }
        else if (ret.compare("000") == 0)
        {
            if (needReActi)
            {
                //LOGINFO("auth state is 000, now redoActivate only once\n");
                //needDoActivate = true;
                //needReActi = false;
                //getLoginType();
                //i = 0;
                continue;
            }
        }
        else
        {
            //sleep(LOGIN_RETRY_WAIT_TIME);
            continue;
        }
    }

    if (i == LOGIN_TRY_TIMES)
    {
        LOGERROR("login failed, up to the max retry times\n");
        mLoginStatus = LoginStatus::LoginFailed;

        if (needDoActivate)
        {
            if (dataCache::getInstance()->getLicense() != "")
            {
                ret = m_activateErrCode;
            }
            else
            {
                LOGERROR("doActivate already try all, but failed\n");
                ret = "1" + m_loginType1ActiErrCode + "-" \
                    + "2" + m_loginType2ActiErrCode + "-" \
                    + "3" + m_loginType3ActiErrCode;
            }
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

    TokenResponse tokenResponse;
    ret = XMLParse::tokenParse(response.c_str(), &tokenResponse);
    if (ret != 0)
    {
        LOGERROR("XMLParse::tokenParse error\n");
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
    LOGINFO("checkTokenThread start!!!\n");

    Login *plogin = (Login *)param;

    while (1)
    {
        sleep(CHECK_TOKEN_TIME_INTERVAL);
        plogin->checkToken();
    }

    LOGINFO("checkTokenThread end!!!\n");

    return 0;
}

int Login::startCheckToken()
{
    if (m_isCheckTokenThreadRunning)
    {
        LOGWARN("Check token thread has already been started\n");
        return 0;
    }

    baseThread::startThread(Login::checkTokenThread, Login::getInstance());

    m_isCheckTokenThreadRunning = true;
    return 0;
}


