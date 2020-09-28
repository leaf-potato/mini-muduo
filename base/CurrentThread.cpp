//
// Created by zrzzzlll on 2020/9/24.
//
#include "CurrentThread.h"
#include "Thread.h"
#include <stdio.h>
#include <unistd.h>
namespace muduo{
    namespace CurrentThread{
        /*
         * 对线程的全局变量进行初始化
         */
        __thread int t_cachedTid = 0;
        __thread char t_tidString[32];
        __thread int t_tidStringLength = 6;
        __thread const char* t_threadName = "unknown";
        void cachedTid(){
            if(t_cachedTid == 0){
                t_cachedTid = detail::getTid();
                /*
                 * snprintf函数使用:
                 * 将字符串格式化写入到另一个字符串，返回值为写入字符串的长度
                 * 为了防止字符串溢出，需要指出目标字符串的长度
                 * */
                t_tidStringLength = snprintf(t_tidString, sizeof(t_tidString), "%5d ",
                                             t_cachedTid);
            }
        }
        bool isMainThread(){
            /*
             * 判断是否为主线程的方式是判断当前线程的id是否和进程的id相同
             * 在多线程环境下进程的id其实主线程的id
             */
            return getTid() == ::getpid();
        }
    }
}