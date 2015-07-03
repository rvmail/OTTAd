#include "loginParse.h"
#include "base/utils/log.h"
#include "../../thirdparty/tinyxml/tinyxml.h"

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
        LOG(ERROR) << "initParse input null pointer";
        return -1;
    }

    LoginResponse *res = (LoginResponse *)dst;

    TiXmlDocument xmlDoc;
    xmlDoc.Parse(src);
    LOG(DEBUG) << "request xml: " << src;
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
                    LOG(DEBUG) << "state=" << res->state;
                }
                else
                {
                    LOG(ERROR) << "state no value";
                    return -1;
                }
            }
            else
            {
                LOG(ERROR) << "state is NULL";
                return -1;
            }

            pChildNode = pNode->FirstChildElement("userId");
            if (pChildNode != NULL)
            {
                p = pChildNode->GetText();
                if (p != NULL)
                {
                    res->userId.assign(p);
                    LOG(DEBUG) << "userId=" << res->userId;
                }
            }

            pChildNode = pNode->FirstChildElement("accountId");
            if (pChildNode != NULL)
            {
                p = pChildNode->GetText();
                if (p != NULL)
                {
                    res->accountId.assign(p);
                    LOG(DEBUG) << "accountId=" << res->accountId;
                }
            }

            pChildNode = pNode->FirstChildElement("password");
            if (pChildNode != NULL)
            {
                p = pChildNode->GetText();
                if (p != NULL)
                {
                    res->password.assign(p);
                    LOG(DEBUG) << "password=" << res->password;
                }
            }

            pChildNode = pNode->FirstChildElement("customerCategory");
            if (pChildNode != NULL)
            {
                p = pChildNode->GetText();
                if (p != NULL)
                {
                    res->customerCategory.assign(p);
                    LOG(DEBUG) << "customerCategory=" << res->customerCategory;
                }
            }

            pChildNode = pNode->FirstChildElement("regionId");
            if (pChildNode != NULL)
            {
                p = pChildNode->GetText();
                if (p != NULL)
                {
                    res->regionId.assign(p);
                    LOG(DEBUG) << "regionId=" << res->regionId;
                }
            }

            pChildNode = pNode->FirstChildElement("templateId");
            if (pChildNode != NULL)
            {
                p = pChildNode->GetText();
                if (p != NULL)
                {
                    res->templateId.assign(p);
                    LOG(DEBUG) << "templateId=" << res->templateId;
                }
            }

            pChildNode = pNode->FirstChildElement("resultCode");
            if (pChildNode != NULL)
            {
                p = pChildNode->GetText();
                if (p != NULL)
                {
                    res->resultCode.assign(p);
                    LOG(DEBUG) << "resultCode=" << res->resultCode;
                }
            }

            pChildNode = pNode->FirstChildElement("token");
            if (pChildNode != NULL)
            {
                p = pChildNode->GetText();
                if (p != NULL)
                {
                    res->token.assign(p);
                    LOG(DEBUG) << "token=" << res->token;
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
                            LOG(DEBUG) << "url=" << address.url;
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
            LOG(ERROR) << "online is NULL";
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
        LOG(ERROR) << "RootElement is NULL";
        return -1;
    }

    return 0;
}
