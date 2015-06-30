#ifndef __INITPARSE_H__
#define __INITPARSE_H__

#include "baseParse.h"
#include <iostream>

struct InitResponse
{
    InitResponse()
    {
        status = -1;
    }

    int status;
    std::string deviceid;
};

class initParse : public baseParse
{
public:
    initParse(void);
    ~initParse(void);

    int parse(const char *src, void *dst);
};

#endif // !__INITPARSE_H__

