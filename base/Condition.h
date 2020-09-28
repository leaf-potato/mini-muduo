//
// Created by zrzzzlll on 2020/9/28.
//

#ifndef MINI_MUDUO_BASE_CONDITION_H_
#define MINI_MUDUO_BASE_CONDITION_H_
#include "Mutex.h"
#include <pthread.h>
namespace muduo{
    class Condition:public nocopyable{
        public:
            explicit Condition(MutexLock& mutex):mutex_(mutex){
                pthread_cond_init(&cond_, nullptr);
            }
            ~Condition(){
                pthread_cond_destroy(&cond_);
            }
            void wait(){
                pthread_cond_wait(&cond_,mutex_.getPthreadMutex());
            }
            void notify(){
                pthread_cond_signal(&cond_);
            }
            void notifyAll(){
                pthread_cond_broadcast(&cond_);
            }
        private:
            /*
             * 这里的MutexLock必须是引用或者指针类型，使用引用类型较好
             * Condition中对MutexLock的修改必须要影响到外部初始化的MutexLock
             */
            MutexLock& mutex_;
            pthread_cond_t cond_;
    };
}

#endif //MINI_MUDUO_BASE_CONDITION_H_
