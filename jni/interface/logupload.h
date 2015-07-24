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
* @date
*/

#ifndef __LOGUPLAOD_H__
#define __LOGUPLOAD_H__

#include <iostream>

class LogUpload
{
public:
    static LogUpload* getInstance();
    ~LogUpload(void);

    void init(void);
    int startUpload(void);

private:
    LogUpload(void);
    static LogUpload *m_instance;
    static void *upload(void *param);
    void getConfig(void);

    int comress2file(const char *srcFileName, const char *dstFileName);

    std::string m_logFile;
    std::string m_compressLogFile;
    std::string m_serverAddr;
    bool m_isUploading;
    int m_loginType;
};

#endif


