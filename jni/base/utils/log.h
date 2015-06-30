/****************************************************************************
 **
 ** Copyright (C) 2013 iCNTV Corporation and/or its subsidiary(-ies).
 ** All rights reserved.
 **
 ** This file is part of the iCNTV Platform Terminal System.
 **
 ** This confidential and proprietary software may be used only as
 ** authorised by a licensing agreement from iCNTV Limited
 **
 ** The entire notice above must be reproduced on all authorised copies
 ** and copies may only be made to the extent permitted by a licensing
 ** agreement from iCNTV Limited.
 **
 ** Contact :  Jiang Lu (jiang.lu@icntv.tv)
 **
 ****************************************************************************/

#ifndef __LOG_H__
#define __LOG_H__

#include <string>
#include <sstream>
#include <android/log.h>

using std::ostream;

#define LOG_TAG         "epgsdk"

#define LL_ERROR        ANDROID_LOG_ERROR
#define LL_WARNING      ANDROID_LOG_WARN
#define LL_INFO         ANDROID_LOG_INFO
#define LL_FATALERR     ANDROID_LOG_FATAL
#define LL_DBGINFO      ANDROID_LOG_DEBUG

class logstream : public std::ostream {
    logstream(const logstream&);
    const logstream& operator=(const logstream&);

public:
    explicit logstream(int level, int line, const char* file,
                       const char* function);

    ~logstream();

    logstream& l_value() {
        return *this;
    }

    // strstreambuf标准接口
    std::stringbuf *rdbuf() const {
        return ((std::stringbuf *) &streambuf_);
    }

    std::string str() {
        return (streambuf_.str());
    }

private:
    std::stringbuf streambuf_;

    int level;
    int line;
    const char *file;
    const char *function;
};

inline logstream::logstream(int level, int line, const char* file,
                            const char* function)
        : ostream(&streambuf_),
          streambuf_(),
          level(level),
          line(line),
          file(file),
          function(function) {
    switch (level) {
        default:
        case LL_INFO:
            l_value() << "\033[32;5m";
            break;
        case LL_DBGINFO:
            l_value() << "\033[36;5m";
            break;
        case LL_WARNING:
            l_value() << "\033[35;5m";
            break;
        case LL_ERROR:
            l_value() << "\033[31;5m";
            break;
        case LL_FATALERR:
            l_value() << "\033[37;41;5m";
            break;
    }
}

inline logstream::~logstream() {
    l_value() << "\033[0;5m";

    //if  ((level == LL_ERROR) || (level == LL_FATALERR))
    	__android_log_write(level, LOG_TAG, str().c_str());
}

#ifndef __FUNCTION__
#define __FUNCTION__ NULL
#endif // __FUNCTION__
//--------------------------------------------------------
// 以下define定义了几个临时变量。这些临时变量在语句结束
// 时析构，导致调用logstream<level>的析构函数，在析构函数
// 中调用write_log()接口写日志
//--------------------------------------------------------

//----------------------------------------------------------
// 注意以下定义的log接口都是一些临时变量，C++标准草案中
// 临时变量是r-value，不能有non-const引用，
// 因此在VC.NET 7.1中你会看到loginfo << "X"调用的是成员操作符
// ostream::operator<<(const void*)，而不是非成员的
// operator<<(ostream&,const char*)，这导致loginfo << "X"输出
// 一个内存地址，而非期望的字符串。
// 我这里的解决方法是：由于r-value可以调用成员函数，成员函数
// 可以返回non-const引用，因此这里的log接口是临时变量
// non-const引用。另一种解决方法是这样用：
// const_cast<logstream&>( loginfo ) << "X";
// VC 6.0和VC .NET 7.0对临时变量缺省选择non-const引用（但不
// 符合C++规范），因此没有这个问题。
// 关于临时变量更详细的讨论参见这里：
// http://groups.google.com/groups?hl=zh-CN&lr=&ie=UTF-8&threadm=77q8ju8cqfg11td4qnn24i9unqp54801in%404ax.com&rnum=1&prev=/groups%3Fselm%3D77q8ju8cqfg11td4qnn24i9unqp54801in%25404ax.com
//----------------------------------------------------------

//--------------------------------------------------------
// 致命错误。程序即将退出
//--------------------------------------------------------
#define logfatal  logstream(LL_FATALERR, __LINE__, __FILE__, __FUNCTION__).l_value()

//--------------------------------------------------------
// 一般错误。某个模块功能错误，但其它部模块不受影响，
// 程序还能继续工作。
//--------------------------------------------------------
#define logerr    logstream(LL_ERROR, __LINE__, __FILE__, __FUNCTION__).l_value()

//--------------------------------------------------------
// 警告信息。需要提醒用户注意的信息，比如对某个接口传递的
// 调用参数不正确，内存溢出，或设备驱动收到一个不能识别的参数。
// 警告和一般错误的区别在于一般错误是程序的逻辑出现问题，
// 警告则是程序本身的自我保护，是正确的逻辑
//--------------------------------------------------------
#define logwarn   logstream(LL_WARNING, __LINE__, __FILE__, __FUNCTION__).l_value()

//--------------------------------------------------------
// 一般信息。报告目前的状态
//--------------------------------------------------------
#define loginfo   logstream(LL_INFO, __LINE__, __FILE__, __FUNCTION__).l_value()

//--------------------------------------------------------
// 调试信息。有助于程序员调试使用的信息。
//--------------------------------------------------------
#define logdbg    logstream(LL_DBGINFO, __LINE__, __FILE__, __FUNCTION__).l_value()

#define LOG_FATAL() \
        logstream(LL_FATALERR, __LINE__, __FILE__, __FUNCTION__).l_value()

#define LOG_ERROR() \
        logstream(LL_ERROR, __LINE__, __FILE__, __FUNCTION__).l_value()

#define LOG_WARN() \
        logstream(LL_WARNING, __LINE__, __FILE__, __FUNCTION__).l_value()

#define LOG_INFO() \
        logstream(LL_INFO, __LINE__, __FILE__, __FUNCTION__).l_value()

#define LOG_DEBUG() \
        logstream(LL_DBGINFO, __LINE__, __FILE__, __FUNCTION__).l_value()

#define LOG(severity) LOG_ ## severity()

#define LOG_IF(severity, condition) \
        !(condition) ? (void) 0 : LOG(severity)

#endif
