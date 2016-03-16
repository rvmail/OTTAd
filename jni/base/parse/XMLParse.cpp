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

#include "XMLParse.h"
#include "debug.h"
#include "tinyxml.h"

XMLParse::XMLParse(void)
{
}

XMLParse::~XMLParse(void)
{
}

int XMLParse::bootParse(const char *src, BootResponse &resp)
{
    if (src == NULL)
    {
        LOGERROR("bootParse input null pointer\n");
        return -1;
    }

    TiXmlDocument xmlDoc;
    xmlDoc.Parse(src);

    TiXmlElement *pRoot = xmlDoc.RootElement();
    if (!pRoot)
    {
        LOGERROR("RootElement is NULL\n");
        return -1;
    }

    //find "addressList"
    TiXmlElement *pNode = pRoot->FirstChildElement("addressList");
    if (pNode)
    {
        const char *p = NULL;

        TiXmlElement *pAddressNode = pNode->FirstChildElement("address");
        while (pAddressNode != NULL)
        {
            ServerAddress address;

            p = pAddressNode->Attribute("type");
            if (p != NULL)
            {
                address.type.assign(p);

                p = pAddressNode->Attribute("url");
                if (p != NULL)
                {
                    address.url.assign(p);
                }

                p = pAddressNode->Attribute("name");
                if (p != NULL)
                {
                    address.name.assign(p);
                    resp.addressList.insert(make_pair(p, address));

                    LOGDEBUG("type(%s), url(%s), name(%s)\n", \
                            address.type.c_str(), address.url.c_str(), address.name.c_str());
                }
                else
                {
                    resp.addressList.insert(make_pair(address.type, address));
                }
            }

            pAddressNode = pAddressNode->NextSiblingElement("address");
        }
    }
    else
    {
        LOGERROR("Can't find addressList\n");
        return -1;
    }

    return 0;
}

int XMLParse::initParse(const char *src, void *dst)
{
    if (src == NULL || dst == NULL)
    {
        LOGERROR("initParse input null pointer\n");
        return -1;
    }

    InitResponse *res = (InitResponse *)dst;

    TiXmlDocument xmlDoc;
    xmlDoc.Parse(src);

    TiXmlElement *pRoot = xmlDoc.RootElement();
    if (!pRoot)
    {
        LOGERROR("RootElement is NULL\n");
        return -1;
    }

    //find "online"
    TiXmlElement *pNode = pRoot->FirstChildElement("online");
    if (pNode)
    {
        const char *p = NULL;

        //find resultCode
        TiXmlElement *pChildNode = pNode->FirstChildElement("resultCode");
        if (pChildNode)
        {
            p = pChildNode->GetText();
            if (p)
            {
                res->resultCode = atoi(p);
            }
        }
        else
        {
            LOGERROR("Can't find resultCode\n");
            //return -1;
        }

        //find deviceId
        pChildNode = pNode->FirstChildElement("deviceId");
        if (pChildNode)
        {
            p = pChildNode->GetText();
            if (p)
            {
                res->deviceid.assign(p);
            }
        }
        else
        {
            LOGERROR("Can't find deviceId\n");
            //return -1;
        }

        //find state
        pChildNode = pNode->FirstChildElement("state");
        if (pChildNode)
        {
            p = pChildNode->GetText();
            if (p)
            {
                res->state.assign(p);
                LOGWARN("WAO! find state=%s\n", p);
            }
        }

        //find time
        pChildNode = pNode->FirstChildElement("time");
        if (pChildNode)
        {
            p = pChildNode->GetText();
            if (p)
            {
                res->time.assign(p);
                LOGWARN("WAO! find time=%s\n", p);
            }
        }

        //find message
        pChildNode = pNode->FirstChildElement("message");
        if (pChildNode)
        {
            p = pChildNode->GetText();
            if (p)
            {
                res->message.assign(p);
                LOGWARN("WAO! find message=%s\n", p);
            }
        }
    }
    else
    {
        LOGERROR("Can't find online\n");
        return -1;
    }

    return 0;
}

int XMLParse::loginParse(const char *src, void *dst)
{
    if (src == NULL || dst == NULL)
    {
        LOGERROR("initParse input null pointer\n");
        return -1;
    }

    LoginResponse *res = (LoginResponse *)dst;

    TiXmlDocument xmlDoc;
    xmlDoc.Parse(src);
    TiXmlElement *pRoot = xmlDoc.RootElement();

    if (pRoot)
    {
        //find online
        TiXmlElement *pNode = pRoot->FirstChildElement("online");
        if (pNode)
        {
            const char *p = NULL;
            TiXmlElement *pChildNode = pNode->FirstChildElement("state");
            if (pChildNode)
            {
                p = pChildNode->GetText();
                if (p)
                {
                    res->state.assign(p);
                    LOGDEBUG("state=%s\n", res->state.c_str());
                }
                else
                {
                    LOGERROR("state no value\n");
                    return -1;
                }
            }
            else
            {
                LOGERROR("state is NULL\n");
                return -1;
            }

            pChildNode = pNode->FirstChildElement("userId");
            if (pChildNode != NULL)
            {
                p = pChildNode->GetText();
                if (p != NULL)
                {
                    res->userId.assign(p);
                    LOGDEBUG("userId=%s\n", res->userId.c_str());
                }
            }

            pChildNode = pNode->FirstChildElement("accountId");
            if (pChildNode != NULL)
            {
                p = pChildNode->GetText();
                if (p != NULL)
                {
                    res->accountId.assign(p);
                    LOGDEBUG("accountId=%s\n", res->accountId.c_str());
                }
            }

            pChildNode = pNode->FirstChildElement("password");
            if (pChildNode != NULL)
            {
                p = pChildNode->GetText();
                if (p != NULL)
                {
                    res->password.assign(p);
                    LOGDEBUG("password=%s\n", res->password.c_str());
                }
            }

            pChildNode = pNode->FirstChildElement("customerCategory");
            if (pChildNode != NULL)
            {
                p = pChildNode->GetText();
                if (p != NULL)
                {
                    res->customerCategory.assign(p);
                    LOGDEBUG("customerCategory=%s\n", res->customerCategory.c_str());
                }
            }

            pChildNode = pNode->FirstChildElement("regionId");
            if (pChildNode != NULL)
            {
                p = pChildNode->GetText();
                if (p != NULL)
                {
                    res->regionId.assign(p);
                    LOGDEBUG("regionId=%s\n", res->regionId.c_str());
                }
            }

            pChildNode = pNode->FirstChildElement("templateId");
            if (pChildNode != NULL)
            {
                p = pChildNode->GetText();
                if (p != NULL)
                {
                    res->templateId.assign(p);
                    LOGDEBUG("templateId=%s\n", res->templateId.c_str());
                }
            }

            pChildNode = pNode->FirstChildElement("resultCode");
            if (pChildNode != NULL)
            {
                p = pChildNode->GetText();
                if (p != NULL)
                {
                    res->resultCode.assign(p);
                    LOGDEBUG("resultCode=%s\n", res->resultCode.c_str());
                }
            }

            pChildNode = pNode->FirstChildElement("token");
            if (pChildNode != NULL)
            {
                p = pChildNode->GetText();
                if (p != NULL)
                {
                    res->token.assign(p);
                    LOGDEBUG("token=%s\n", res->token.c_str());
                }
            }

            pChildNode = pNode->FirstChildElement("addressList");
            if (pChildNode != NULL)
            {
                TiXmlElement *pAddressNode = pChildNode->FirstChildElement("address");
                while (pAddressNode != NULL)
                {
                    ServerAddress address;
                    int type = -1;
                    p = pAddressNode->Attribute("type");
                    if (p != NULL)
                    {
                        address.type.assign(p);

                        p = pAddressNode->Attribute("url");
                        if (p != NULL)
                        {
                            address.url.assign(p);
                        }

                        p = pAddressNode->Attribute("name");
                        if (p != NULL)
                        {
                            address.name.assign(p);
                            res->serverList.insert(make_pair(p, address));
                            LOGDEBUG("type(%s), url(%s), name(%s)\n", \
                                    address.type.c_str(), address.url.c_str(), address.name.c_str());
                        }
                        else
                        {
                            res->serverList.insert(make_pair(address.type, address));
                        }
                    }

                    pAddressNode = pAddressNode->NextSiblingElement("address");
                }
            }

            //when state is 93, time is not ok
            pChildNode = pNode->FirstChildElement("time");
            if (pChildNode != NULL)
            {
                p = pChildNode->GetText();
                if (p != NULL)
                {
                    res->time.assign(p);
                    LOGWARN("time=%s\n", res->time.c_str());
                }
            }

            pChildNode = pNode->FirstChildElement("message");
            if (pChildNode != NULL)
            {
                p = pChildNode->GetText();
                if (p != NULL)
                {
                    res->message.assign(p);
                    LOGWARN("message=%s\n", res->message.c_str());
                }
            }
        }
        else
        {
            LOGERROR("online is NULL\n");
            return -1;
        }

        pNode = pRoot->FirstChildElement("update");
        if (pNode != NULL)
        {
            //Todo
        }
    }
    else
    {
        LOGERROR("RootElement is NULL\n");
        return -1;
    }

    return 0;
}

int XMLParse::tokenParse(const char *src, void *dst)
{
    if (src == NULL || dst == NULL)
    {
        LOGERROR("tokenParse input null pointer\n");
        return -1;
    }

    TokenResponse *res = (TokenResponse *)dst;

    TiXmlDocument xmlDoc;
    xmlDoc.Parse(src);

    TiXmlElement *pRoot = xmlDoc.RootElement();
    if (!pRoot)
    {
        LOGERROR("RootElement is NULL\n");
        return -1;
    }

    //find "Response"
    TiXmlElement *pNode = pRoot->FirstChildElement("Response");
    if (pNode)
    {
        const char *p = NULL;

        //find responseCode
        TiXmlElement *pChildNode = pNode->FirstChildElement("responseCode");
        if (pChildNode)
        {
            p = pChildNode->GetText();
            if (p)
            {
                res->respCode = atoi(p);
            }
            else
            {
                LOGERROR("responseCode value is NULL\n");
                return -1;
            }
        }
        else
        {
            LOGERROR("Can't find responseCode\n");
            return -1;
        }

        //find responseTime
        pChildNode = pNode->FirstChildElement("responseTime");
        if (pChildNode)
        {
            p = pChildNode->GetText();
            if (p)
            {
                res->respTime.assign(p);
            }
        }
    }
    else
    {
        LOGERROR("Can't find Response\n");
        return -1;
    }

    return 0;
}


