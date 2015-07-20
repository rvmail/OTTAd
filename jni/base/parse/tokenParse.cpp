#include "tokenParse.h"
#include "debug.h"
#include "tinyxml.h"

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
