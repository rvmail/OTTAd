#ifndef __LOGINPARSE_H__
#define __LOGINPARSE_H__

#include "baseParse.h"
#include <iostream>
#include <map>

using namespace std;

struct ServerAddress
{
    string type;
    string url;
};
typedef map<string, ServerAddress> MapServerList;

struct LoginResponse
{
    string state;
    string userId;
    string accountId;
    string password;
    string customerCategory;
    string regionId;
    string templateId;
    string resultCode;
    string token;
    MapServerList serverList;
    string time;
    string message;
};

class loginParse : public baseParse
{
public:
    loginParse(void);
    ~loginParse(void);

    int parse(const char *src, void *dst);
};

#endif

