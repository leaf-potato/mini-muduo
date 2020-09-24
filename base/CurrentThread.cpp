//
// Created by zrzzzlll on 2020/9/24.
//
#include "CurrentThread.h"
namespace muduo{
    namespace CurrentThread{
        /*
         * 对线程的全局变量进行初始化
         */
        __thread int t_cachedTid = 0;
        __thread char t_tidString[32];
        __thread int t_tidStringLength = 6;
        __thread const char* t_threadName = "unknown";
    }
}