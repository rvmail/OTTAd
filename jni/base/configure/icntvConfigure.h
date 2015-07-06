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
* @author   @icntv.tv
* @date
*/
/************************************************************************/
/* function：provide config file read and write, and custom icntv base function
*  return : value -1 is failed, other value success
*/
/************************************************************************/

#ifndef __ICNTVCONFIGURE_H__
#define __ICNTVCONFIGURE_H__

class icntvConfigure
{
public:
    static icntvConfigure* getInstance();
    ~icntvConfigure();

    int getLoginServer(char *serverAddr, int size);

    int getDeviceID(char *id, int size);
    int setDeviceID(const char *id);

    int getPlatformID(char *id, int size);

    int getStrValue(const char *section, const char *key , char *keyvalue, \
                    const int bufsize, const char *filePath);
    int getIntValue(const char *section, const char *key, \
                    const char *filePath);
    int setKeyValue(const char *section, const char *key , \
                    const char *keyvalue, const char *filePath);
private:
    icntvConfigure();
    int getFilePath(char *szFile, const char *filePath = NULL);

private:
    static icntvConfigure *m_pInstance;
    char szFile[128];
    int m_intValue;
};

#endif
