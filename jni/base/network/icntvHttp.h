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

#ifndef __ICNTVHTTP_H__
#define __ICNTVHTTP_H__

#include <iostream>
#include <list>
using namespace std;

#include "thirdparty/curl/include/curl/curl.h"

/************************************************************************/
/* class httpresponse
   function : provide http response data, need allocate new memory before
   used. This class used only curl request.
*/
/************************************************************************/
class httpResponse
{
public:
    httpResponse()
    {
        m_nLength = 0;
        m_pResponse.clear();
    }

    char *allocate(int size)
    {
        char *data = NULL;
        if (size > 0)
        {
            data = new char[size + 1];
            memset(data, 0, size + 1);
            m_pResponse.push_back(data);
            m_nLength += size;
        }
        return data;
    }

    int getLength()
    {
        return m_nLength;
    }

    int getResponse(char *data)
    {
        int nLength = 0;
        char *p = data;
        list<char*>::const_iterator itTmp;
        for (list<char*>::iterator it = m_pResponse.begin(); it != m_pResponse.end();)
        {
            char *data = *it;
            int size = strlen(data);
            memcpy(p + nLength, data, size);
            nLength += size;

            delete data;
            data = NULL;
            it = m_pResponse.erase(it);
        }
        return nLength;
    }

    ~httpResponse()
    {
        if (!m_pResponse.empty())
        {
            for (list<char*>::iterator it = m_pResponse.begin(); it != m_pResponse.end();)
            {
                delete *it;
                it = m_pResponse.erase(it);
            }
        }
    }

private:
    int m_nLength;
    list<char*> m_pResponse;
};

class icntvHttp
{
public:
    icntvHttp(void);
    ~icntvHttp(void);

    int get(const char *request, httpResponse *response);
    int post(const char *head, const char *data, httpResponse *response);

    /**
    * @name          getData
    * @param
    * @return        0: OK
    *               -1: if curlget is not CURLE_OK, return -1
    *               -2: ERR_MALLOC
    * @description
    */
    int getData(string host, string path, string query, string &response);
    int postData(string host, string path, string query, string &response);

protected:
    static size_t write_func(char *buffer, size_t size, size_t nitems, void *outstream);

    CURL* m_pCurl;
};

class icntvHttps : public icntvHttp
{
public:
    icntvHttps();
    ~icntvHttps();

    int get(const char *request, httpResponse *response);
private:

};



#endif // !__ICNTVHTTP_H__

