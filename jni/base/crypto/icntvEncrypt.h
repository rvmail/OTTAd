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

#ifndef __ICNTFENCRYPT_H__
#define __ICNTFENCRYPT_H__

#include <string>

enum AesPaddingType
{
    NoPadding,
    PKCS5Padding,
};

enum AesResultType
{
    Original,
    Base64,
    Hexadecimal,
};

class icntvEncrypt
{
public:
    icntvEncrypt(void);
    ~icntvEncrypt(void);

    std::string md5Encrypt(const std::string src);
    std::string aesEncrypt(const std::string src, const std::string key);
    std::string aesDecrypt(const std::string src, const std::string key);

private:
    std::string hex_encode(const std::string &src);
    std::string hex_decode(const std::string &src);
};

#endif // !__ICNTFENCRYPT_H__

