//
// Created by zrzzzlll on 2020/9/29.
//
#include "Exception.h"
#include "CurrentThread.h"
using namespace muduo;
Exception::Exception(const std::string& msg)
    :message_(msg),stack_(CurrentThread::stackTrace(false)){
}
