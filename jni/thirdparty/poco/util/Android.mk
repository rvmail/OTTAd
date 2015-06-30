#############################################################################
##
## Copyright (C) 2013 iCNTV Corporation and/or its subsidiary(-ies).
## All rights reserved.
##
## This file is part of the iCNTV Platform Terminal System.
##
## This confidential and proprietary software may be used only as
## authorised by a licensing agreement from iCNTV Limited
##
## The entire notice above must be reproduced on all authorised copies
## and copies may only be made to the extent permitted by a licensing
## agreement from iCNTV Limited.
##
## Contact :  Jiang Lu (jiang.lu@icntv.tv)
##
#############################################################################
OLD_LOCAL_PATH := $(LOCAL_PATH)

###########################################################
# libpoco_util.a
###########################################################
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := poco_util

LOCAL_MODULE_FILENAME := libpoco_util

LOCAL_CFLAGS += -fvisibility=hidden
LOCAL_CFLAGS += -DPOCO_ANDROID \
                -DPOCO_NO_FPENVIRONMENT \
                -DPOCO_NO_WSTRING \
                -DPOCO_NO_SHAREDMEMORY \

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include

LOCAL_C_INCLUDES := $(LOCAL_EXPORT_C_INCLUDES)

LOCAL_SRC_FILES := \
    src/AbstractConfiguration.cpp \
    src/Application.cpp \
    src/ConfigurationMapper.cpp \
    src/ConfigurationView.cpp \
    src/FilesystemConfiguration.cpp \
    src/HelpFormatter.cpp \
    src/IniFileConfiguration.cpp \
    src/IntValidator.cpp \
    src/LayeredConfiguration.cpp \
    src/LoggingConfigurator.cpp \
    src/LoggingSubsystem.cpp \
    src/MapConfiguration.cpp \
    src/Option.cpp \
    src/OptionCallback.cpp \
    src/OptionException.cpp \
    src/OptionProcessor.cpp \
    src/OptionSet.cpp \
    src/PropertyFileConfiguration.cpp \
    src/RegExpValidator.cpp \
    src/ServerApplication.cpp \
    src/Subsystem.cpp \
    src/SystemConfiguration.cpp \
    src/Timer.cpp \
    src/TimerTask.cpp \
    src/Validator.cpp \
    src/XMLConfiguration.cpp \

#LOCAL_LDLIBS := -lstdc++ -lsupc++

LOCAL_STATIC_LIBRARIES := poco_foundation poco_xml

include $(BUILD_STATIC_LIBRARY)

LOCAL_PATH := $(OLD_LOCAL_PATH)

###########################################################
$(call import-module, thirdparty/poco/foundation)
$(call import-module, thirdparty/poco/xml)