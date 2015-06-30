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

#include "SystemClock.h"

#include <time.h>
#include <sys/time.h>

int64_t SystemClock::uptimeMillis() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (static_cast<int64_t>(ts.tv_sec) * 1000)
            + (static_cast<int64_t>(ts.tv_nsec) / 1000000);
}

int64_t SystemClock::currentTimeMillis() {
    struct timeval tv;
    gettimeofday(&tv, 0);
    return ((static_cast<int64_t>(tv.tv_sec) * 1000)
            + (static_cast<int64_t>(tv.tv_usec) / 1000));
}
