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
*
*/

#ifndef DEVICEINFO_H_
#define DEVICEINFO_H_

#include <string>

#ifdef __cplusplus
extern "C"
{
#endif

std::string getMac(int type);
std::string getMacByFile();
std::string getMacBySocket();
unsigned long long getMacInNum();
unsigned long long convertMac2Num(std::string mac);
int setNetType(int type);

#ifdef __cplusplus
};
#endif

#endif /* DEVICEINFO_H_ */
