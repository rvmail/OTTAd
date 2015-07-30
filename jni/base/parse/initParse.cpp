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
