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
* @author   zml@icntv.tv
* @date     2015-06-30
*/

/****************************************************************
 * login SDK interface
 ****************************************************************/

#ifndef __OTTLOGIN_H__
#define __OTTLOGIN_H__

#include <iostream>
using namespace std;

/**
* @name         ICNTV_Login_deviceLogin
* @param        path[in]: the path to the configuration files
* @return
* @description  device login
*/
string ICNTV_Login_deviceLogin(string path);

/**
* @name         sdkExit
* @param        void
* @return       bool true success, false failed
* @description  quit from the SDK
*/
bool sdkExit(void);

/**
* @name         getVersion
* @param        version[out]
* @return       0 success, -1 failed
* @description  get the version of the SDK
*/
int getVersion(string &version);

/**
* @name         getDeviceID
* @param        deviceID[out]
* @return       0 success, -1 failed
* @description  get device ID
*/
int getDeviceID(string &deviceID);

/**
* @name         getUserID
* @param        userID[out]
* @return       0 success, -1 failed
* @description  get user ID
*/
int getUserID(string &userID);

/**
* @name         getTemplateID
* @param        templateID[out]
* @return       0 success, -1 failed
* @description  get the template ID
*/
int getTemplateID(string &templateID);

/**
* @name         getPlatformID
* @param        platformID[out]
* @return       0 success, -1 failed
* @description  get the platform ID
*/
int getPlatformID(string &platformID);

/**
* @name         getServerAddress
* @param        type[in]: type of the server
*               "EPG"
* @param        platformID[out]
* @return       0 success, -1 failed
* @description  get the platform ID
*/
int getServerAddress(string type, string &serverAddr);

#endif

