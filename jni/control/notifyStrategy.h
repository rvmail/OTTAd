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
/* 功能说明：负责与服务端的消息通讯，如心跳等
*/
/************************************************************************/
#ifndef __NOTIFYSTRATEGE_H__
#define __NOTIFYSTRATEGE_H__



class notifyStrategy
{
public:
	notifyStrategy(void);
	~notifyStrategy(void);

	void heartbeat();
};

#endif // !__NOTIFYSTRATEGE_H__

