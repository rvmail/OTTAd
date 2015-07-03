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
#include "base/utils/log.h"
#include "base/utils/URI.h"

/************************************************************************/
/* class icntvHttp */
/************************************************************************/
icntvHttp::icntvHttp(void) : m_pCurl(NULL)
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

int icntvHttp::get(const char *request, httpResponse *response)
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

        CURLcode retCode = curl_easy_perform(m_pCurl);
        if (retCode != CURLE_OK)
        {
            const char* pError = curl_easy_strerror(retCode);
            if (pError != NULL)
            {
                nRet = -1;
                LOG(ERROR) << "http error, " << pError;
            }
        }
    }

    return nRet;
}

int icntvHttp::post(const char *head, const char *data, httpResponse *response)
{
    int nRet = 0;

    if (m_pCurl != NULL && data != NULL)
    {
        curl_easy_setopt(m_pCurl, CURLOPT_VERBOSE, 1L);
        curl_easy_setopt(m_pCurl, CURLOPT_URL, head);
        curl_easy_setopt(m_pCurl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(m_pCurl, CURLOPT_POST, 1L);
        curl_easy_setopt(m_pCurl, CURLOPT_POSTFIELDS, data);
        curl_easy_setopt(m_pCurl, CURLOPT_WRITEFUNCTION, &icntvHttp::write_func);
        curl_easy_setopt(m_pCurl, CURLOPT_WRITEDATA, response);
        curl_easy_setopt(m_pCurl, CURLOPT_NOSIGNAL, 1L);;

        CURLcode retCode = curl_easy_perform(m_pCurl);
        if (retCode != CURLE_OK)
        {
            const char* pError = curl_easy_strerror(retCode);
            if (pError != NULL)
            {
                nRet = -1;
                LOG(ERROR) << "http error, " << pError;
            }
        }
    }

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

    LOG(DEBUG) << "Request URL: " << url.toString();

    ret = get(url.toString().c_str(), &resp);
    if (ret != 0)
    {
        LOG(ERROR) << "http.get() error!";
        return -1;
    }

    int len = resp.getLength();
    LOG(DEBUG) << "resp.getLength() return " << len;
    char *buf = new char[len + 1];
    if (buf == NULL)
    {
        LOG(ERROR) << "new char[] error!";
        return -2;
    }
    buf[len] = 0;

    resp.getResponse(buf);

    response.assign(buf, len);

    delete [] buf;

    LOG(DEBUG) << response;
    LOG(DEBUG) << "response length=" << response.length();

    return 0;
}

int icntvHttp::postData(string host, string path, string query, string &response)
{
    //build URL of the request
    int ret;
    httpResponse resp;
    URI url(host);

    url.setPath(url.getPath() + path);

    LOG(DEBUG) << "Request URL: " << url.toString();

    ret = post(url.toString().c_str(), query.c_str(), &resp);
    if (ret != 0)
    {
        LOG(ERROR) << "http.post() error!";
        return -1;
    }

    int len = resp.getLength();
    LOG(DEBUG) << "resp.getLength() return " << len;
    char *buf = new char[len + 1];
    if (buf == NULL)
    {
        LOG(ERROR) << "new char[] error!";
        return -2;
    }
    buf[len] = 0;

    resp.getResponse(buf);

    response.assign(buf, len);

    delete [] buf;

    LOG(DEBUG) << response;
    LOG(DEBUG) << "response length=" << response.length();

    return 0;
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
                LOG(ERROR) << "https error, " << pError;
            }
        }
    }

    return nRet;
}

