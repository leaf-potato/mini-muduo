//
// Created by zrzzzlll on 2020/9/28.
//
#include "Thread.h"
#include <unistd.h>
#include <sys/syscall.h>
namespace muduo{
    namespace detail{
        pid_t getTid(){
            return static_cast<pid_t>(::syscall(SYS_gettid));
        }
    }
}