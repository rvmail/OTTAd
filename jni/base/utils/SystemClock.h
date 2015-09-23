/****************************************************************************
 **
 ** Copyright (C) 2013 iCNTV Corporation and/or its subsidiary(-ies).
 ** All rights reserved.
 **
 ** This file is part of the iCNTV Platform Terminal System.
 **
 ** This confidential and proprietary software may be used only as
 ** authorised by a licensing agreement from iCNTV Limited
 **
 ** The entire notice above must be reproduced on all authorised copies
 ** and copies may only be made to the extent permitted by a licensing
 ** agreement from iCNTV Limited.
 **
 ** Author: jianglu
 **
 ****************************************************************************/

#ifndef ICNTV_SYSTEMCLOCK_H_
#define ICNTV_SYSTEMCLOCK_H_

#include <cstdint>
#include <string>

class SystemClock
{
public:
    /**
     * 获得开机以来的秒数
     */
    static int64_t uptimeMillis();

    /**
     * 获得 1970 年以来的秒数
     */
    static int64_t currentTimeMillis();

    static std::string getTime();
};

#endif /* ICNTV_SYSTEMCLOCK_H_ */
