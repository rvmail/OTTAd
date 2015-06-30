APP_PLATFORM := android-10
NDK_TOOLCHAIN_VERSION = 4.9

# Build for all arch ndk supported
APP_ABI := armeabi-v7a  #x86 armeabi 

# enable rtti
APP_STL := gnustl_static

APP_CPPFLAGS += -std=gnu++11 -frtti -fexceptions -Wno-literal-suffix

NDK_MODULE_PATH := jni
