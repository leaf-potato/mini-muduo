//
// Created by zrzzzlll on 2020/9/28.
//

#ifndef MINI_MUDUO_BASE_BLOCKINGQUEUE_H_
#define MINI_MUDUO_BASE_BLOCKINGQUEUE_H_
#include "Mutex.h"
#include "Condition.h"
#include <deque>
namespace muduo{

    template <typename T>
    class BlockingQueue: public nocopyable{
        public:
            BlockingQueue():mutex_(),notEmpty_(mutex_),deque_(){
            }
            void put(const T& value){
                MutexLockGuard lock(mutex_);
                deque_.push_back(value);
                notEmpty_.notify();
            }
            void put(T&& value){
                MutexLockGuard lock(mutex_);
                deque_.push_back(std::move(value));
                notEmpty_.notify();
            }
            T take(){
                MutexLockGuard lock(mutex_);
                while(deque_.empty()){
                    notEmpty_.wait();
                }
                T front(std::move(deque_.front()));
                deque_.pop_front();
                return front;
            }
            size_t size()const{
                MutexLockGuard lock(mutex_);
                return deque_.size();
            }
        private:
            mutable MutexLock mutex_;
            Condition notEmpty_;
            std::deque<T>deque_;
    };
}
#endif //MINI_MUDUO_BASE_BLOCKINGQUEUE_H_
