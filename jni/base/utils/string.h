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
* @author   li.he@icntv.tv
* @date     2014-10-10
*/

#ifndef __STRING_H__
#define __STRING_H__

#include <string>
#include <cctype>
#include <algorithm>
#include <list>

using namespace std;

inline bool compareCaseInsensitive(string strFirst, string strSecond)
{
  transform(strFirst.begin(), strFirst.end(), strFirst.begin(), ::tolower);
  transform(strSecond.begin(), strSecond.end(), strSecond.begin(), ::tolower);
  if(strFirst == strSecond) return true; else return false;
}

inline list<string> split(string str, string separator)
{
		list<string> result;
		int cutAt;
		while( (cutAt = str.find_first_of(separator)) != str.npos )
		{
			if(cutAt > 0)
			{
				result.push_back(str.substr(0, cutAt));
			}
			str = str.substr(cutAt + 1);
		}

		if(str.length() > 0)
		{
			result.push_back(str);
		}

		return result;
}

#endif
