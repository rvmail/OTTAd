#include "loginParse.h"
#include "debug.h"
#include "tinyxml.h"

loginParse::loginParse(void)
{
}

loginParse::~loginParse(void)
{
}

int loginParse::parse(const char *src, void *dst)
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
                            LOGDEBUG("type(%s), url(%s)\n", address.type.c_str(), address.url.c_str());
                        }

                        p = pAddressNode->Attribute("name");
                        if (p != NULL)
                        {
                            res->serverList.insert(make_pair(p, address));
                        }
                        else
                        {
                            res->serverList.insert(make_pair(address.type, address));
                        }
                    }

                    pAddressNode = pAddressNode->NextSiblingElement("address");
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
