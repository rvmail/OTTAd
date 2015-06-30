#
# Thrift 0.8.0 static and shared C++ libraries
#

OLD_LOCAL_PATH := $(LOCAL_PATH)
LOCAL_PATH := $(call my-dir)
SRC_PATH := src
INCLUDE_PATH := $(LOCAL_PATH)/include

THRIFT_FILENAME := libthrift
THRIFT_CPPFLAGS := -DHAVE_CONFIG_H -D__GLIBC__

ifeq ($(BOOST_ROOT),)
#BOOST_ROOT := $(INCLUDE_PATH)/boost/include
endif

THRIFT_SRC_FILES := \
    $(SRC_PATH)/Thrift.cpp \
    $(SRC_PATH)/TApplicationException.cpp \
    $(SRC_PATH)/async/TAsyncChannel.cpp \
    $(SRC_PATH)/async/TAsyncProtocolProcessor.cpp \
    $(SRC_PATH)/processor/PeekProcessor.cpp \
    $(SRC_PATH)/transport/TSocket.cpp \
    $(SRC_PATH)/transport/TTransportException.cpp \
    $(SRC_PATH)/transport/TZlibTransport.cpp \
    $(SRC_PATH)/transport/TSocketPool.cpp \
    $(SRC_PATH)/transport/TServerSocket.cpp \
    $(SRC_PATH)/transport/THttpClient.cpp \
    $(SRC_PATH)/transport/TTransportUtils.cpp \
    $(SRC_PATH)/transport/THttpTransport.cpp \
    $(SRC_PATH)/transport/TBufferTransports.cpp \
    $(SRC_PATH)/transport/TFileTransport.cpp \
    $(SRC_PATH)/transport/TSimpleFileTransport.cpp \
    $(SRC_PATH)/transport/THttpServer.cpp \
    $(SRC_PATH)/transport/TFDTransport.cpp \
    $(SRC_PATH)/concurrency/Util.cpp \
    $(SRC_PATH)/concurrency/PosixThreadFactory.cpp \
    $(SRC_PATH)/concurrency/TimerManager.cpp \
    $(SRC_PATH)/concurrency/Mutex.cpp \
    $(SRC_PATH)/concurrency/ThreadManager.cpp \
    $(SRC_PATH)/concurrency/Monitor.cpp \
    $(SRC_PATH)/server/TThreadPoolServer.cpp \
    $(SRC_PATH)/server/TServer.cpp \
    $(SRC_PATH)/server/TThreadedServer.cpp \
    $(SRC_PATH)/server/TSimpleServer.cpp \
    $(SRC_PATH)/protocol/TDebugProtocol.cpp \
    $(SRC_PATH)/protocol/TDenseProtocol.cpp \
    $(SRC_PATH)/protocol/TJSONProtocol.cpp \
    $(SRC_PATH)/protocol/TBase64Utils.cpp \

    # not supported yet
    # async/TEvhttpServer.cpp \
    # async/TEvhttpClientChannel.cpp \
    # transport/TSSLSocket.cpp \
    # transport/TSSLServerSocket.cpp \
    # server/TNonblockingServer.cpp \
	# $(SRC_PATH)/VirtualProfiling.cpp \

##
## Shared library directives
##

#include $(CLEAR_VARS)

#LOCAL_MODULE := thrift_shared

#LOCAL_C_INCLUDES := $(INCLUDE_PATH) $(BOOST_ROOT)

#LOCAL_MODULE_FILENAME := $(THRIFT_FILENAME)
#LOCAL_CPPFLAGS := $(THRIFT_CPPFLAGS)
#LOCAL_SRC_FILES := $(THRIFT_SRC_FILES)

#LOCAL_LDLIBS := -lz

#LOCAL_EXPORT_C_INCLUDES := $(INCLUDE_PATH)
#LOCAL_EXPORT_CPPFLAGS := $(THRIFT_CPPFLAGS)

#include $(BUILD_SHARED_LIBRARY)

#
# Static library directives
#

include $(CLEAR_VARS)

LOCAL_MODULE := thrift

LOCAL_C_INCLUDES := $(INCLUDE_PATH) $(BOOST_ROOT)

LOCAL_MODULE_FILENAME := $(THRIFT_FILENAME)
LOCAL_CPPFLAGS := $(THRIFT_CPPFLAGS)
LOCAL_SRC_FILES := $(THRIFT_SRC_FILES)

LOCAL_EXPORT_C_INCLUDES := $(INCLUDE_PATH)
LOCAL_EXPORT_CPPFLAGS := $(THRIFT_CPPFLAGS)

LOCAL_STATIC_LIBRARIES := boost

include $(BUILD_STATIC_LIBRARY)

LOCAL_PATH := $(OLD_LOCAL_PATH)

$(call import-module, thirdparty/boost)