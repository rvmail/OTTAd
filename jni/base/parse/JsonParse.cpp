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
*
*/

#include "JsonParse.h"
#include "document.h"
#include "debug.h"

JsonParse::JsonParse(void)
{
}


JsonParse::~JsonParse(void)
{
}

bool JsonParse::parseActivate(const char *src, void *dst)
{
    if (src == NULL || dst == NULL)
    {
        LOGERROR("param error\n");
        return false;
    }

    ActivateResponse *resp = (ActivateResponse *)dst;

    rapidjson::Document doc;
    doc.Parse<rapidjson::kParseDefaultFlags>(src);
    if (doc.HasParseError())
    {
        LOGERROR("doc.Parse error: %s\n", doc.GetParseError());
        return false;
    }

    if (doc.IsObject())
    {
        // returnCode
        if (!doc["returnCode"].IsNull())
        {
            resp->returnCode.assign(doc["returnCode"].GetString());
        }
        else
        {
            LOGERROR("returnCode is empty\n");
            return false;
        }

        //icntvid
        if (!doc["icntvid"].IsNull())
        {
            resp->icntvid.assign(doc["icntvid"].GetString());
        }
        else
        {
            LOGERROR("icntvid is empty\n");
            //return false;
        }

        //appCode
        if (!doc["appCode"].IsNull())
        {
            resp->appCode.assign(doc["appCode"].GetString());
        }
        else
        {
            LOGERROR("appCode is empty\n");
            //return false;
        }

        //description
        if (!doc["description"].IsNull())
        {
            resp->description.assign(doc["description"].GetString());
        }

        return true;
    }

    return false;
}

