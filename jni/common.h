#ifndef __COMMON_H__
#define __COMMON_H__

#define GIT_VERSION "20160127-aba577b"

#if (SDK_TYPE == 1)  //ANDROID_JAR
    #define SDK_VERSION "V1.2.1"
#elif (SDK_TYPE == 2)  //ANDROID_PURESDK
    #define SDK_VERSION "V1.0.0"
#else  //others
    #define SDK_VERSION "V1.0.0"
#endif

const int NUM_4 = 4;
const int NUM_8 = 8;
const int NUM_16 = 16;
const int NUM_32 = 32;
const int NUM_64 = 64;
const int NUM_128 = 128;
const int NUM_256 = 256;

#endif // !__COMMON_H__

