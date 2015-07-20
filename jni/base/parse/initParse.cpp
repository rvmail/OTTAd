#include "initParse.h"
#include "tinyxml.h"
#include "debug.h"

initParse::initParse(void)
{
}


initParse::~initParse(void)
{
}

int initParse::parse(const char *src, void *dst)
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
                res->status = atoi(p);
            }
        }
        else
        {
            LOGERROR("Can't find resultCode\n");
            return -1;
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
            return -1;
        }
    }
    else
    {
        LOGERROR("Can't find online\n");
        return -1;
    }

    return 0;
}
