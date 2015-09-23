#ifndef __COMMON_H__
#define __COMMON_H__

#define GIT_VERSION "20150918-7445e6f"

#if (SDK_TYPE == 1)  //ANDROID_JAR
    #define SDK_VERSION "1.0.7"
#elif (SDK_TYPE == 2)  //ANDROID_PURESDK
    #define SDK_VERSION "1.0.0"
#else  //others
    #define SDK_VERSION "1.0.0"
#endif

const int NUM_4 = 4;
const int NUM_8 = 8;
const int NUM_16 = 16;
const int NUM_32 = 32;
const int NUM_64 = 64;
const int NUM_128 = 128;
const int NUM_256 = 256;

#endif // !__COMMON_H__

