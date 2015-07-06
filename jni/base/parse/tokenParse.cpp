#include "base/utils/log.h"
#include "tokenParse.h"
#include "../../thirdparty/tinyxml/tinyxml.h"

tokenParse::tokenParse(void)
{
}


tokenParse::~tokenParse(void)
{
}

int tokenParse::parse(const char *src, void *dst)
{
    if (src == NULL || dst == NULL)
    {
        LOG(ERROR) << "tokenParse input null pointer";
        return -1;
    }

    TokenResponse *res = (TokenResponse *)dst;

    TiXmlDocument xmlDoc;
    xmlDoc.Parse(src);
    LOG(DEBUG) << "request xml: " << src;

    TiXmlElement *pRoot = xmlDoc.RootElement();
    if (!pRoot)
    {
        LOG(ERROR) << "RootElement is NULL";
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
                LOG(ERROR) << "responseCode value is NULL";
                return -1;
            }
        }
        else
        {
            LOG(ERROR) << "Can't find responseCode";
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
        LOG(ERROR) << "Can't find Response";
        return -1;
    }

    return 0;
}
