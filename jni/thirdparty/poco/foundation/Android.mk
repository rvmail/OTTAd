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
# libpoco_foundation.a
###########################################################
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := poco_foundation

LOCAL_MODULE_FILENAME := libpoco_foundation

LOCAL_CFLAGS += -fvisibility=hidden
LOCAL_CFLAGS += -DPOCO_ANDROID \
                -DPOCO_NO_FPENVIRONMENT \
                -DPOCO_NO_WSTRING \
                -DPOCO_NO_SHAREDMEMORY \

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include

LOCAL_C_INCLUDES := $(LOCAL_EXPORT_C_INCLUDES)

LOCAL_SRC_FILES := \
    src/ASCIIEncoding.cpp \
    src/AbstractObserver.cpp \
    src/ActiveDispatcher.cpp \
    src/ArchiveStrategy.cpp \
    src/Ascii.cpp \
    src/AsyncChannel.cpp \
    src/AtomicCounter.cpp \
    src/Base64Decoder.cpp \
    src/Base64Encoder.cpp \
    src/BinaryReader.cpp \
    src/BinaryWriter.cpp \
    src/Bugcheck.cpp \
    src/ByteOrder.cpp \
    src/Channel.cpp \
    src/Checksum.cpp \
    src/Condition.cpp \
    src/Configurable.cpp \
    src/ConsoleChannel.cpp \
    src/CountingStream.cpp \
    src/DateTime.cpp \
    src/DateTimeFormat.cpp \
    src/DateTimeFormatter.cpp \
    src/DateTimeParser.cpp \
    src/Debugger.cpp \
    src/DeflatingStream.cpp \
    src/DigestEngine.cpp \
    src/DigestStream.cpp \
    src/DirectoryIterator.cpp \
    src/DirectoryWatcher.cpp \
    src/DynamicAny.cpp \
    src/DynamicAnyHolder.cpp \
    src/Environment.cpp \
    src/ErrorHandler.cpp \
    src/Event.cpp \
    src/EventArgs.cpp \
    src/Exception.cpp \
    src/FPEnvironment.cpp \
    src/File.cpp \
    src/FileChannel.cpp \
    src/FileStream.cpp \
    src/FileStreamFactory.cpp \
    src/Format.cpp \
    src/Formatter.cpp \
    src/FormattingChannel.cpp \
    src/Glob.cpp \
    src/Hash.cpp \
    src/HashStatistic.cpp \
    src/HexBinaryDecoder.cpp \
    src/HexBinaryEncoder.cpp \
    src/InflatingStream.cpp \
    src/Latin1Encoding.cpp \
    src/Latin9Encoding.cpp \
    src/LineEndingConverter.cpp \
    src/LocalDateTime.cpp \
    src/LogFile.cpp \
    src/LogStream.cpp \
    src/Logger.cpp \
    src/LoggingFactory.cpp \
    src/LoggingRegistry.cpp \
    src/MD4Engine.cpp \
    src/MD5Engine.cpp \
    src/Manifest.cpp \
    src/MemoryPool.cpp \
    src/MemoryStream.cpp \
    src/Message.cpp \
    src/Mutex.cpp \
    src/NamedEvent.cpp \
    src/NamedMutex.cpp \
    src/NestedDiagnosticContext.cpp \
    src/Notification.cpp \
    src/NotificationCenter.cpp \
    src/NotificationQueue.cpp \
    src/NullChannel.cpp \
    src/NullStream.cpp \
    src/NumberFormatter.cpp \
    src/NumberParser.cpp \
    src/Path.cpp \
    src/PatternFormatter.cpp \
    src/Pipe.cpp \
    src/PipeImpl.cpp \
    src/PipeStream.cpp \
    src/PriorityNotificationQueue.cpp \
    src/Process.cpp \
    src/PurgeStrategy.cpp \
    src/RWLock.cpp \
    src/Random.cpp \
    src/RandomStream.cpp \
    src/RefCountedObject.cpp \
    src/RegularExpression.cpp \
    src/RotateStrategy.cpp \
    src/Runnable.cpp \
    src/SHA1Engine.cpp \
    src/Semaphore.cpp \
    src/SharedLibrary.cpp \
    src/SharedMemory.cpp \
    src/SignalHandler.cpp \
    src/SimpleFileChannel.cpp \
    src/SplitterChannel.cpp \
    src/Stopwatch.cpp \
    src/StreamChannel.cpp \
    src/StreamConverter.cpp \
    src/StreamCopier.cpp \
    src/StreamTokenizer.cpp \
    src/String.cpp \
    src/StringTokenizer.cpp \
    src/SynchronizedObject.cpp \
    src/SyslogChannel.cpp \
    src/Task.cpp \
    src/TaskManager.cpp \
    src/TaskNotification.cpp \
    src/TeeStream.cpp \
    src/TemporaryFile.cpp \
    src/TextBufferIterator.cpp \
    src/TextConverter.cpp \
    src/TextEncoding.cpp \
    src/TextIterator.cpp \
    src/Thread.cpp \
    src/ThreadLocal.cpp \
    src/ThreadPool.cpp \
    src/ThreadTarget.cpp \
    src/TimedNotificationQueue.cpp \
    src/Timer.cpp \
    src/Timespan.cpp \
    src/Timestamp.cpp \
    src/Timezone.cpp \
    src/Token.cpp \
    src/URI.cpp \
    src/URIStreamFactory.cpp \
    src/URIStreamOpener.cpp \
    src/UTF16Encoding.cpp \
    src/UTF8Encoding.cpp \
    src/UTF8String.cpp \
    src/UUID.cpp \
    src/UUIDGenerator.cpp \
    src/Unicode.cpp \
    src/UnicodeConverter.cpp \
    src/Void.cpp \
    src/Windows1252Encoding.cpp \
    src/adler32.c \
    src/compress.c \
    src/crc32.c \
    src/deflate.c \
    src/infback.c \
    src/inffast.c \
    src/inflate.c \
    src/inftrees.c \
    src/pcre_chartables.c \
    src/pcre_compile.c \
    src/pcre_exec.c \
    src/pcre_fullinfo.c \
    src/pcre_globals.c \
    src/pcre_maketables.c \
    src/pcre_newline.c \
    src/pcre_ord2utf8.c \
    src/pcre_study.c \
    src/pcre_tables.c \
    src/pcre_try_flipped.c \
    src/pcre_ucd.c \
    src/pcre_valid_utf8.c \
    src/pcre_xclass.c \
    src/trees.c \
    src/zutil.c \

#LOCAL_LDLIBS := -lstdc++ -lsupc++

include $(BUILD_STATIC_LIBRARY)

LOCAL_PATH := $(OLD_LOCAL_PATH)