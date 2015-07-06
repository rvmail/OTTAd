#ifndef __TOKENPARSE_H__
#define __TOKENPARSE_H__

#include "baseParse.h"
#include <iostream>

struct TokenResponse
{
    TokenResponse()
    {
        respCode = -1;
    }

    int respCode;
    std::string respTime;
};

class tokenParse : public baseParse
{
public:
    tokenParse(void);
    ~tokenParse(void);

    int parse(const char *src, void *dst);
};

#endif // !__INITPARSE_H__

