#include "icntvParse.h"
#include "initParse.h"
#include "loginParse.h"

icntvParse::icntvParse(void)
{
}


icntvParse::~icntvParse(void)
{
}

void icntvParse::parse( PARSETYPE type, const char* src, void* dst )
{
	switch (type)
	{
	case unkown:
		break;

	case init:
		{
			initParse initPar;
			initPar.parse(src, dst);
		}
		break;

	case login:
		{
			loginParse loginPar;
			loginPar.parse(src, dst);
		}
		break;

	default:
		break;
	}
}
