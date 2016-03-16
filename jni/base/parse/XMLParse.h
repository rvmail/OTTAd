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

#ifndef __XMLPARSE_H__
#define __XMLPARSE_H__

#include <iostream>
#include <map>

using namespace std;

struct InitResponse
{
    InitResponse()
    {
        resultCode = -1;
    }

    int resultCode;
    std::string deviceid;
    std::string state;
    std::string time;
    std::string message;
};

struct ServerAddress
{
    string type;
    string url;
    string name;
};
typedef map<string, ServerAddress> MapServerList;

struct BootResponse
{
    MapServerList addressList;
};

struct LoginResponse
{
    string state;
    string userId;
    string accountId;
    string password;
    string customerCategory;
    string regionId;
    string templateId;
    string resultCode;
    string token;
    MapServerList serverList;
    string time;
    string message;
};

struct TokenResponse
{
    TokenResponse()
    {
        respCode = -1;
    }

    int respCode;
    std::string respTime;
};

class XMLParse
{
public:
    XMLParse(void);
    ~XMLParse(void);

    static int bootParse(const char *src, BootResponse &resp);
    static int initParse(const char *src, void *dst);
    static int loginParse(const char *src, void *dst);
    static int tokenParse(const char *src, void *dst);
};

#endif

