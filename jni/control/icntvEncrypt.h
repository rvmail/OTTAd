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
* @author   chu.lei@icntv.tv
* @date     2014-9-19
*/
/************************************************************************/
/* ����˵�����ṩ�ӽ��ܹ���
*/
/************************************************************************/
#ifndef __ICNTFENCRYPT_H__
#define __ICNTFENCRYPT_H__

#include <string>

#define algorithm_md5 "MD5"
#define 	algorithm_sha1 "SHA1"

class icntvEncrypt
{
public:
	icntvEncrypt(void);
	icntvEncrypt(const std::string encryption);
	~icntvEncrypt(void);

	void setToken(const char* token);
	std::string encrypt(const std::string src);
	int decrypt(const char* src, char* dst);

private:
	std::string md5Encrypt(const std::string src);

private:
	std::string encryption;
};

#endif // !__ICNTFENCRYPT_H__

