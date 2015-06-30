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
* @date     2014-9-19
*/
/************************************************************************/
/* 功能说明：数据解析工厂方法，提供数据解析对外接口
*/
/************************************************************************/
#ifndef __ICNTVPARSE_H__
#define __ICNTVPARSE_H__

enum PARSETYPE
{
	unkown = 0,
	init,
	login
};

class icntvParse
{
public:
	icntvParse(void);
	~icntvParse(void);

	void parse(PARSETYPE type, const char* src, void* dst);
};

#endif // !__ICNTVPARSE_H__

