//
// Created by zrzzzlll on 2020/9/24.
//

#ifndef MINI_MUDUO_BASE_MUTEXLOCK_H_
#define MINI_MUDUO_BASE_MUTEXLOCK_H_
#include "nocopyable.h"
#include "CurrentThread.h"
#include <pthread.h>
#include <cassert>
/*
 * 使用RAII手法对mutex进行封装
 */
namespace muduo{
    class MutexLock: public nocopyable{
        public:
            MutexLock():holder_(0){
                pthread_mutex_init(&mutex_, nullptr);
            }
            void lock(){
                pthread_mutex_lock(&mutex_);
                assignHolder();
            }
            void unlock(){
                holder_ = 0;
                pthread_mutex_unlock(&mutex_);
            }
            bool isLockedByThisThread(){
                return holder_ == CurrentThread::getTid();
            }
            void assignLocked(){
                assert(isLockedByThisThread());
            }
            pthread_mutex_t* getPthreadMutex(){
                return &mutex_;
            }
            ~MutexLock(){
                assert(holder_ == 0);
                pthread_mutex_destroy(&mutex_);
            }
        private:
            void assignHolder(){
                holder_ = CurrentThread::getTid();
            }
            void unassignHolder(){
                holder_ = 0;
            }
            pthread_mutex_t mutex_;
            pid_t holder_;
    };
    class MutexLockGuard: public nocopyable{
        public:
            /*
             * 避免隐式转换
             */
            explicit MutexLockGuard(MutexLock& mutex):mutex_(mutex){
                mutex_.lock();
            }
            ~MutexLockGuard(){
                mutex_.unlock();
            }
        private:
            MutexLock& mutex_;
    };

    /*
     * 避免MutexLockGuard(mutex)这类错误的发生
     * 产生了一个临时对象又马上销毁了
     */
    #define MutexLockGuard(x) static_assert(false, "error missing guard var name");
}
#endif //MINI_MUDUO_BASE_MUTEXLOCK_H_
