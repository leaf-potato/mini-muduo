//
// Created by zrzzzlll on 2020/9/24.
//
#include "CurrentThread.h"
#include "Thread.h"
#include <stdio.h>
/*
 * 声明了三个函数用于获取当前线程的调用堆栈
 * int backtrace(void **array, int size);
 * char **backtrace_symbols(void *const *array, int size);
 * void backtrace_symbols_fd(void *const *array, int size, int fd);
 */
#include <cxxabi.h>
#include <execinfo.h>
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
        /*
         * 获取当前线程的调用栈
         * 使用glibc的backtrace来获取栈上函数的实际名称
         */
        std::string stackTrace(bool demangle){
            std::string stack;
            const int max_frames = 200;
            void* frames[max_frames];
            /*
             * int backtrace(void **array, int size);
             * array:从backtrace中获取的函数指针将存储在该指针数组中
             * size:是array数组的大小
             * 返回值:array数组中实际返回的大小
             */
            int nptrs = ::backtrace(frames,max_frames);
            /*
             * char **backtrace_symbols(void *const *array, int size);
             * array:应该是backtrace函数返回的指针数组
             * size:是backtrace函数的返回值
             * 返回值:
             * 一个指向字符串数组的指针, 它的大小同 buffer 相同.
             * 每个字符串包含了一个相对于buffer中对应元素的可打印信息.
             * 它包括函数名，函数的偏移地址,和实际的返回地址
             * 返回值指向的内存是通过malloc动态内存分配的，
             * 所以在使用完成结束之后需要调用free来释放内存空间
             * */
            char** strings = ::backtrace_symbols(frames,nptrs);
            if(strings){
                size_t len = 256;
                char* demangled = demangle ? static_cast<char*>(::malloc(len)) :
                    nullptr;
                for(int i = 1;i < nptrs;i++){
                    if(demangle){
                        // 是否将backtrace_symbols获取的字符串转为可读
                        char* left_par = nullptr;
                        char* plus = nullptr;
                        // 遍历每一个字符串
                        for(char* p = strings[i];*p;p++){
                            if(*p == '(')
                                left_par = p;
                            else if(*p == '+')
                                plus = p;
                        }
                        if(left_par && plus){
                            *plus = '\0';
                            int status = 0;
                            char* ret = abi::__cxa_demangle(left_par + 1,demangled,
                                                            &len,&status);

                        }
                    }
                    stack.append(strings[i]);
                    stack.push_back('\n');
                }
                free(demangled);
                free(strings);
            }
            return stack;
        }
    }
}