//
// Created by zrzzzlll on 2020/9/28.
//

#ifndef MINI_MUDUO_BASE_THREAD_H_
#define MINI_MUDUO_BASE_THREAD_H_
#include "Atomic.h"
#include "CountDownLatch.h"
#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>
#include <functional>
namespace muduo{
    namespace detail{
        /*
         * 通过linux中的syscall函数来获得每一个线程的id
         * gettid()获得id具备唯一性
         */
        pid_t getTid();
    }
    class Thread: public nocopyable{
        public:
            using ThreadFunc = std::function<void()>;
            explicit Thread(ThreadFunc func,const std::string& name = std::string());
            void start();
            int join();
            bool isStarted()const{
                return started_;
            }
            int getTid()const{
                return tid_;
            }
            const std::string& getThreadName()const{
                return name_;
            }
            static int getThreadCreatedNumber(){
                return numCreated_.get();
            }
            ~Thread();
        private:
            void setDefaultName();
            bool started_;
            bool joined_;
            std::string name_;
            pthread_t pthreadId_;
            pid_t tid_;
            ThreadFunc func_;
            CountDownLatch latch_;
        static AtomicInt32 numCreated_;
    };
}



#endif //MINI_MUDUO_BASE_THREAD_H_
