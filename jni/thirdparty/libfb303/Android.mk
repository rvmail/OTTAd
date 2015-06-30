OLD_LOCAL_PATH := $(LOCAL_PATH)
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := fb303

LOCAL_MODULE_FILENAME := libfb303

LOCAL_C_INCLUDES :=  $(LOCAL_PATH)/src/gen-cpp\
			$(LOCAL_PATH)/src
                    
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_C_INCLUDES)

LOCAL_SRC_FILES := $(call all-cpp-files-under,src)

LOCAL_STATIC_LIBRARIES := libthrift

include $(BUILD_STATIC_LIBRARY)

LOCAL_PATH := $(OLD_LOCAL_PATH)

$(call import-module, thirdparty/libthrift)
