#ifndef __COMMON_H__
#define __COMMON_H__

#define GIT_VERSION "20160608-6d2923d"

#if (SDK_TYPE == 1)  //ANDROID_JAR
    #define SDK_VERSION "V2.0.1"
#elif (SDK_TYPE == 2)  //ANDROID_PURESDK
    #define SDK_VERSION "V1.0.0"
#else  //others
    #define SDK_VERSION "V1.0.0"
#endif

// APP_KEY
#if (APP_KEY == 1)
    #define APPKEY    "FutureTv"
#elif (APP_KEY == 3)
    #define APPKEY    "UN99QQGCW_HZ"
#elif (APP_KEY == 4)
    #define APPKEY    "UN98DZCJ_SH"
#elif (APP_KEY == 5)
    #define APPKEY    "UN97NGDY_BJ"
#elif (APP_KEY == 6)
    #define APPKEY    "UN96BWEG"
#elif (APP_KEY == 7)
    #define APPKEY    "UN95UTSDK"
#else
    #define APPKEY    ""
#endif

// PLATFORM
#if (PLATFORM == 1)
    #define PLATFORM_ID    "icntv01ifap"
#elif (PLATFORM == 2)
    #define PLATFORM_ID    "NewTV104MiGuShiXun"
#elif (PLATFORM == 3)
    #define PLATFORM_ID    "UN99QQGCW_HZ"
#elif (PLATFORM == 4)
    #define PLATFORM_ID    "UN98DZCJ_SH"
#elif (PLATFORM == 5)
    #define PLATFORM_ID    "UN97NGDY_BJ"
#elif (PLATFORM == 6)
    #define PLATFORM_ID    "UN96BWEG"
#elif (PLATFORM == 7)
    #define PLATFORM_ID    "UN95UTSDK"
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

