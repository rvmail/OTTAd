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
* @author   chu.lei@icntv.tv
* @date     2014-9-22
*/
/************************************************************************/
/* ����˵�����ṩ���ݻ��湦��
*/
/************************************************************************/
#ifndef __DATACACHE_H__
#define __DATACACHE_H__

#include <iostream>
using namespace std;

class dataCache
{
public:
	~dataCache(void);
	static dataCache* getInstance();
	void setPath(string path);
	string getPath();
	int getPath(char* path);
	void setVersion(string version);
	string getVersion();

protected:
	dataCache(void);

private:
	static dataCache* m_pInstance;

	string mPath;
	string mVersion;
};

#endif // !__DATACACHE_H__

