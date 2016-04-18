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
* zhangmingliang
*/

#ifndef __JSONPARSE_H__
#define __JSONPARSE_H__

#include <iostream>

struct ActivateResponse
{
    std::string icntvid;
    std::string returnCode;
    std::string description;
    std::string appCode;
};

class JsonParse
{
public:
    JsonParse(void);
    ~JsonParse(void);

    static bool parseActivate(const char *src, void *dst);
};

#endif

