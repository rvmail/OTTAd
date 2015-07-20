LOCAL_PATH := $(call my-dir)

###########################################################
## Find all of files under the named directories.
###########################################################
OUTPUT_TYPE=ANDROID_JAR

define all-files-under
$(patsubst ./%,%, \
  $(shell cd $(LOCAL_PATH) ; \
          find $(1) -name "$(2)" -and -not -name ".*") \
 )
endef

define all-cpp-files-under
$(call all-files-under,$(1),*.cpp)
endef

define all-cc-files-under
$(call all-files-under,$(1),*.cc)
endef

define all-c-files-under
$(call all-files-under,$(1),*.c)
endef

define all-S-files-under
$(call all-files-under,$(1),*.S)
endef

define set-git-version
$(shell $(LOCAL_PATH)/set_version.sh)
endef

include $(CLEAR_VARS)

LOCAL_MODULE := ottlogin

LOCAL_C_INCLUDES := $(LOCAL_PATH) \
                    $(LOCAL_PATH)/base \
                    $(LOCAL_PATH)/base/configure \
                    $(LOCAL_PATH)/base/network \
                    $(LOCAL_PATH)/base/parse \
                    $(LOCAL_PATH)/base/utils \
                    $(LOCAL_PATH)/control \
                    $(LOCAL_PATH)/interface \
                    $(LOCAL_PATH)/thirdparty/tinyxml

LOCAL_SRC_FILES := ottlogin.cpp \
                   $(call all-cpp-files-under, base) \
                   $(call all-c-files-under,   base) \
                   $(call all-cpp-files-under, control) \
                   $(call all-cpp-files-under, thirdparty/tinyxml) \
                   $(call all-cpp-files-under, interface)

LOCAL_STATIC_LIBRARIES := poco_foundation \
                          poco_util \
                          libcurl \
                          libthrift \
                          libfb303

LOCAL_LDLIBS     += -fuse-ld=bfd -rdynamic -llog -lz

#-DWLAN_MAC: Wireless
#-DETH_MAC: Ethernet
#-DSET_MAC: Setting type of the network by setNetworkType, default Wireless
LOCAL_CPPFLAGS :=  -DSET_MAC
$(call set-git-version)

ifeq ($(OUTPUT_TYPE), ANDROID_JAR)
	LOCAL_CPPFLAGS += -DSDK_TYPE=1
	LOCAL_CPPFLAGS += -DPURE_SDK
	LOCAL_MODULE_FILENAME := libottlogin
	LOCAL_SRC_FILES += ottlogin_jni.cpp
	include $(BUILD_SHARED_LIBRARY)
endif

$(call import-module, thirdparty/poco/foundation)
$(call import-module, thirdparty/poco/util)
$(call import-module, thirdparty/curl)
$(call import-module, thirdparty/libthrift)
$(call import-module, thirdparty/libfb303)
