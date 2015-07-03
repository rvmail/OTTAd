#include "base/utils/log.h"
#include "initParse.h"
#include "../../thirdparty/tinyxml/tinyxml.h"

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
        LOG(ERROR) << "initParse input null pointer";
        return -1;
    }

    InitResponse *res = (InitResponse *)dst;

    TiXmlDocument xmlDoc;
    xmlDoc.Parse(src);
    LOG(DEBUG) << "request xml: " << src;

    TiXmlElement *pRoot = xmlDoc.RootElement();
    if (!pRoot)
    {
        LOG(ERROR) << "RootElement is NULL";
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
            LOG(ERROR) << "Can't find resultCode";
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
            LOG(ERROR) << "Can't find deviceId";
            return -1;
        }
    }
    else
    {
        LOG(ERROR) << "Can't find online";
        return -1;
    }

    return 0;
}
