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

#include "icntvHttp.h"
#include "base/utils/URI.h"
#include "debug.h"

/************************************************************************/
/* class icntvHttp */
/************************************************************************/
icntvHttp::icntvHttp(void) : m_pCurl(NULL), m_timeout(60)
{
    m_pCurl = curl_easy_init(); // curl init
}

icntvHttp::~icntvHttp(void)
{
    if (m_pCurl != NULL)
    {
        curl_easy_cleanup(m_pCurl); // clean curl handle
        m_pCurl = NULL;
    }
}

size_t icntvHttp::write_func(char *buffer, size_t size, size_t nitems, void *outstream)
{
    if (nitems > 0)
    {
        httpResponse* response = (httpResponse*)outstream;
        char* data = response->allocate(nitems);
        memcpy(data, buffer, nitems);
    }

    return nitems;
}

void icntvHttp::setTimeout(int timeout)
{
    if (timeout > 0)
    {
        m_timeout = timeout;
    }
}

int icntvHttp::get(const char *request, httpResponse *response)
{
    CURLcode retCode = CURLE_UNSUPPORTED_PROTOCOL;  //CURLE_UNSUPPORTED_PROTOCOL default

    if (m_pCurl != NULL)
    {
        curl_easy_setopt(m_pCurl, CURLOPT_VERBOSE, 1L);
        curl_easy_setopt(m_pCurl, CURLOPT_URL, request); // set curl request
        curl_easy_setopt(m_pCurl, CURLOPT_TIMEOUT, m_timeout); // set timeout
        curl_easy_setopt(m_pCurl, CURLOPT_FOLLOWLOCATION, 1L); // follow redirection
        curl_easy_setopt(m_pCurl, CURLOPT_WRITEFUNCTION, &icntvHttp::write_func);
        curl_easy_setopt(m_pCurl, CURLOPT_WRITEDATA, response);
        curl_easy_setopt(m_pCurl, CURLOPT_NOSIGNAL, 1L);

        retCode = curl_easy_perform(m_pCurl);
        if (retCode != CURLE_OK)
        {
            const char* pError = curl_easy_strerror(retCode);
            if (pError != NULL)
            {
                LOGERROR("http get error, %s\n", pError);
            }
        }
    }

    return retCode;
}

int icntvHttp::post(const char *head, const char *data, int datasize, httpResponse *response)
{
    int nRet = 0;
    struct curl_slist *chunk = NULL;

    if (m_pCurl != NULL && data != NULL)
    {
        chunk = curl_slist_append(chunk, "Accept-Encoding:gzip,deflate");
        chunk = curl_slist_append(chunk, m_header1.c_str());
        chunk = curl_slist_append(chunk, m_header2.c_str());
        curl_easy_setopt(m_pCurl, CURLOPT_HTTPHEADER, chunk);

        curl_easy_setopt(m_pCurl, CURLOPT_VERBOSE, 1L);
        curl_easy_setopt(m_pCurl, CURLOPT_URL, head);
        curl_easy_setopt(m_pCurl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(m_pCurl, CURLOPT_POST, 1L);
        curl_easy_setopt(m_pCurl, CURLOPT_POSTFIELDS, data);
        curl_easy_setopt(m_pCurl, CURLOPT_POSTFIELDSIZE, datasize);
        curl_easy_setopt(m_pCurl, CURLOPT_WRITEFUNCTION, &icntvHttp::write_func);
        curl_easy_setopt(m_pCurl, CURLOPT_WRITEDATA, response);
        curl_easy_setopt(m_pCurl, CURLOPT_NOSIGNAL, 1L);;
        curl_easy_setopt(m_pCurl, CURLOPT_TIMEOUT, m_timeout); // set timeout

        CURLcode retCode = curl_easy_perform(m_pCurl);
        if (retCode != CURLE_OK)
        {
            const char* pError = curl_easy_strerror(retCode);
            if (pError != NULL)
            {
                nRet = -1;
                LOGERROR("http post error, %s\n", pError);
            }
        }
    }

    curl_slist_free_all(chunk);
    return nRet;
}

int icntvHttp::getData(string host, string path, string query, string &response)
{
    int ret;
    httpResponse resp;
    URI url(host);

    // If the loginAddr is http://tms.is.ysten.com:8080/yst-tms,
    // the host is "tms.is.ysten.com" and the path is "/yst-tms" when
    // URI(const std::string& uri) was invoked, so we need to get the
    // path first.
    url.setPath(url.getPath() + path);
    url.setQuery(query);

    LOGDEBUG("Get URL: %s\n", url.toString().c_str());

    ret = get(url.toString().c_str(), &resp);
    if (ret != 0)
    {
        LOGERROR("http.get return %d, url:%s\n", ret, url.toString().c_str());
        return ret;
    }

    int len = resp.getLength();
    char *buf = new char[len + 1];
    if (buf == NULL)
    {
        LOGERROR("new char[] error!\n");
        return -1;
    }
    buf[len] = 0;

    resp.getResponse(buf);

    response.assign(buf, len);

    delete [] buf;

    LOGDEBUG("length=%d\n", response.length());
    LOGDEBUG("%s\n", response.c_str());

    return 0;
}

int icntvHttp::postData(string host, string path, const char *data, int datasize, string &response)
{
    //build URL of the request
    int ret;
    httpResponse resp;
    URI url(host);

    url.setPath(url.getPath() + path);

    LOGDEBUG("Post URL: %s\n", url.toString().c_str());
    LOGDEBUG("Post data=%s, size=%d\n", data, datasize);

    ret = post(url.toString().c_str(), data, datasize, &resp);
    if (ret != 0)
    {
        LOGERROR("http.post error!\n");
        return -1;
    }

    int len = resp.getLength();
    char *buf = new char[len + 1];
    if (buf == NULL)
    {
        LOGERROR("new char[] error!\n");
        return -2;
    }
    buf[len] = 0;

    resp.getResponse(buf);

    response.assign(buf, len);

    delete [] buf;

    LOGDEBUG("length=%d\n", response.length());
    LOGDEBUG("%s\n", response.c_str());

    return 0;
}

void icntvHttp::setHeader(string key, string value)
{
    if (key.compare("ID") == 0)
    {
        m_header1 = key + ":";
        m_header1 += value;
    }
    else if (key.compare("UK") == 0)
    {
        m_header2 = key + ":";
        m_header2 += value;
    }
}

/************************************************************************/
/* class icntvHttps */
/************************************************************************/

icntvHttps::icntvHttps()
{

}

icntvHttps::~icntvHttps()
{

}

int icntvHttps::get(const char *request, httpResponse *response)
{
    int nRet = 0;

    if (m_pCurl != NULL)
    {
        curl_easy_setopt(m_pCurl, CURLOPT_VERBOSE, 1L);
        curl_easy_setopt(m_pCurl, CURLOPT_URL, request); // set curl request
        //curl_easy_setopt(m_pCurl, CURLOPT_TIMEOUT, 5); // set timeout
        curl_easy_setopt(m_pCurl, CURLOPT_FOLLOWLOCATION, 1L); // follow redirection
        curl_easy_setopt(m_pCurl, CURLOPT_WRITEFUNCTION, &icntvHttp::write_func);
        curl_easy_setopt(m_pCurl, CURLOPT_WRITEDATA, response);
        curl_easy_setopt(m_pCurl, CURLOPT_NOSIGNAL, 1L);

        curl_easy_setopt(m_pCurl, CURLOPT_SSLVERSION, 3L);
        curl_easy_setopt(m_pCurl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(m_pCurl, CURLOPT_SSL_VERIFYHOST, 0L);

        //curl_easy_setopt(m_pCurl, CURLOPT_SSL_VERIFYPEER, 1L);
        //curl_easy_setopt(m_pCurl, CURLOPT_CAINFO, "capath");

        CURLcode retCode = curl_easy_perform(m_pCurl);
        if (retCode != CURLE_OK)
        {
            const char* pError = curl_easy_strerror(retCode);
            if (pError != NULL)
            {
                nRet = -1;
                LOGERROR("https error, %s\n", pError);
            }
        }
    }

    return nRet;
}

