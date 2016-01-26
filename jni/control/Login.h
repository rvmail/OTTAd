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

#ifndef __LOGIN_H__
#define __LOGIN_H__

#include "base/parse/loginParse.h"

using namespace std;

enum LoginStatus
{
    LoginNot,
    Logining,
    LoginSuccess,
    LoginFailed,
    LoginNetworkFailed,
    LoginForceStop,
    LoginTokenErr,
    LoginUnknown,
};

class Login
{
public:
    static Login* getInstance();
    ~Login(void);

    void init(void);
    string startLogin();
    void stopLogin();

    LoginStatus getLoginStatus(void);
    string getLoginState(void);
    string getToken();
    string getDeviceID();
    string getUserID();
    string getPlatformID();
    string getTemplateID();
    string getServerAddress(string type);

private:
    enum ConfigType
    {
        configLoginAddr,
        configLoginAddrBackup,
        configLogAddr,
        configUpdataAddr,
        configDeviceId,
        configPlatformId,
        configLoginType,
        configMacFile
    };

    enum eLoginType
    {
        LoginActivate,
        LoginAuth
    };

    Login(void);

    int startCheckToken();

    string buildQuery(eLoginType type, string mac);

    void changeLoginServerAddr(int errorCode);
    bool whetherNeedActivate();

    string doActivate();
    string doAuthenticate();

    string getConfigure(ConfigType type);
    int setConfigure(ConfigType type, const string val);

    int checkToken();
    static void *checkTokenThread(void *param);

    int getLoginServerAddr(void);
    void getLoginType(void);
    void setLoginType(void);
    void changeLoginType(void);
    void setActivateErrCode(string err);

    static Login* m_pInstance;

    LoginStatus mLoginStatus;
    string m_loginState;    //store the return value of startLogin,
                            //getLoginStatus will get m_loginState

    string mDeviceId;
    string mUserId;
    string mPlatformId;
    string mTemplateId;
    string mToken;
    MapServerList mServerList;

    bool m_isInit;
    bool m_isCheckTokenStart;

    int m_loginType;    //activate by MAC: 1 wlan, 2 eth, 3 mac file
    string m_macFile;

    string m_serverTime;

    string m_loginType1ActiErrCode;
    string m_loginType2ActiErrCode;
    string m_loginType3ActiErrCode;
    string m_activateErrCode;

    string m_loginServer;         //the address of the TMS
    string m_loginServerBackup;   //used when connect to m_loginServer failed
    bool m_backupServerIsUsed;
};

#endif // !__LOGIN_H__


