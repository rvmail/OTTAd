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

#include <stdio.h>
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

//get current time, format 20150713145028(year month day hour minute second)
std::string SystemClock::getTime()
{
    struct timeval tv;
    struct timezone tz;
    struct tm *p;
    char time[100] = {0};

    gettimeofday(&tv, &tz);
    //printf("tv_sec:%ld\n",tv.tv_sec);
    //printf("tv_usec:%ld\n",tv.tv_usec);
    //printf("tz_minuteswest:%d\n",tz.tz_minuteswest);
    //printf("tz_dsttime:%d\n",tz.tz_dsttime);

//    p = localtime(&tv.tv_sec);
//    sprintf(time, "%04d-%02d-%02d %02d:%02d:%02d.%ld", \
//            1900+p->tm_year, 1+p->tm_mon, p->tm_mday, \
//            p->tm_hour, p->tm_min, p->tm_sec, tv.tv_usec);

    p = localtime(&tv.tv_sec);
    sprintf(time, "%04d%02d%02d%02d%02d%02d", \
                  1900+p->tm_year, 1+p->tm_mon, p->tm_mday, \
                  p->tm_hour, p->tm_min, p->tm_sec);

    std::string t(time);
    return t;
}

