#ifndef __INITPARSE_H__
#define __INITPARSE_H__

#include "baseParse.h"
#include <iostream>

struct InitResponse
{
    InitResponse()
    {
        resultCode = -1;
    }

    int resultCode;
    std::string deviceid;
    std::string state;
    std::string time;
    std::string message;
};

class initParse : public baseParse
{
public:
    initParse(void);
    ~initParse(void);

    int parse(const char *src, void *dst);
};

#endif // !__INITPARSE_H__

