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

    LoginStatus getLoginStatus(void);
    string getLoginState(void);
    string getToken();
    string getDeviceID();
    string getUserID();
    string getPlatformID();
    string getTemplateID();
    string getServerAddress(string type);
    string startLogin();
    void stopLogin();
    int startCheckToken();
    void getLoginType(void);

private:
    enum ConfigType
    {
        configLoginAddr,
        configLogAddr,
        configUpdataAddr,
        configDeviceId,
        configPlatformId,
        configLoginType,
    };

    Login(void);

    /**
    * @name          doActivate
    * @param         no
    * @return        0: OK;
    *                -1: http request error
    *                -2: the response data is not good
    * @description   Activate the terminal, get device ID
    */
    string doActivate();

    /**
    * @name          doAuthenticate
    * @param         no
    * @return        0: OK;
    *                -1: http request error
    *                -2: state=000
    *                -3: other result
    * @description   Authenticate the terminal, get some information
    */
    string doAuthenticate();

    string getConfigure(ConfigType type);
    bool setConfigure(ConfigType type, const string val);

    int checkToken();
    static void *checkTokenThread(void *param);

    void setLoginType(void);
    void changeLoginType(void);

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

    bool m_isCheckTokenStart;
    int m_loginType;    //activate by MAC: 1 wlan, 0 eth
};

#endif // !__LOGIN_H__


