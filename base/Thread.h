//
// Created by zrzzzlll on 2020/9/28.
//

#ifndef MINI_MUDUO_BASE_THREAD_H_
#define MINI_MUDUO_BASE_THREAD_H_
#include <sys/types.h>
namespace muduo{
    namespace detail{
        /*
         * 通过linux中的syscall函数来获得每一个线程的id
         * gettid()获得id具备唯一性
         */
        pid_t getTid();
    }
}



#endif //MINI_MUDUO_BASE_THREAD_H_
