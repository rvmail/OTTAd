#ifndef __COMMON_H__
#define __COMMON_H__

#define GIT_VERSION "20160608-6d2923d"

#if (SDK_TYPE == 1)  //ANDROID_JAR
    #define SDK_VERSION "V2.0.0"
#elif (SDK_TYPE == 2)  //ANDROID_PURESDK
    #define SDK_VERSION "V1.0.0"
#else  //others
    #define SDK_VERSION "V1.0.0"
#endif

// APP_KEY
#if (APP_KEY == 1)
    #define APPKEY    "FutureTv"
#elif (APP_KEY == 2)
    #define APPKEY    ""
#else
    #define APPKEY    ""
#endif

// PLATFORM
#if (PLATFORM == 1)
    #define PLATFORM_ID    "icntv01ifap"
#elif (PLATFORM == 2)
    #define PLATFORM_ID    "NewTV104MiGuShiXun"
#else
    #define PLATFORM_ID    ""
#endif

const int NUM_4 = 4;
const int NUM_8 = 8;
const int NUM_16 = 16;
const int NUM_32 = 32;
const int NUM_64 = 64;
const int NUM_128 = 128;
const int NUM_256 = 256;

#endif // !__COMMON_H__

