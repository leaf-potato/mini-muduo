//
// Created by zrzzzlll on 2020/9/28.
//

#ifndef MINI_MUDUO_BASE_COUNTDOWNLATCH_H_
#define MINI_MUDUO_BASE_COUNTDOWNLATCH_H_
#include "Mutex.h"
#include "Condition.h"
namespace muduo{
    class CountDownLatch: public nocopyable{
        public:
            explicit CountDownLatch(int count);
            void wait();
            void countDown();
            int getCount()const;
        private:
            mutable MutexLock mutex_;
            Condition condition_;
            int count_;
    };
}
#endif //MINI_MUDUO_BASE_COUNTDOWNLATCH_H_
