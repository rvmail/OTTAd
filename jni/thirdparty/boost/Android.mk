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
# libboost.a
###########################################################
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := boost

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include

LOCAL_C_INCLUDES := $(LOCAL_EXPORT_C_INCLUDES)

LOCAL_SRC_FILES := \

#LOCAL_LDLIBS := -lstdc++ -lsupc++

include $(BUILD_STATIC_LIBRARY)

LOCAL_PATH := $(OLD_LOCAL_PATH)