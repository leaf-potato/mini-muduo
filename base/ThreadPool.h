//
// Created by zrzzzlll on 2020/10/4.
//

#ifndef MINI_MUDUO_BASE_THREADPOLL_H_
#define MINI_MUDUO_BASE_THREADPOLL_H_
#include "Thread.h"
#include "Mutex.h"
#include "Condition.h"
#include <deque>
#include <vector>
#include <memory>
namespace muduo{
    class ThreadPool : public nocopyable{
        public:
            using Task = std::function<void ()>;
            /*
             * 只需要提供一个线程池的名字即可创建一个线程池
             */
            explicit ThreadPool(const std::string& nameArg = std::string("ThreadPoll"));
            /*
             * 设置任务队列中最多容纳的任务数量
             * 必须在start()函数之前进行调用
             */
            void setMaxQueueSize(int maxSize){
                maxQueueSize = maxSize;
            }
            /*
             * 线程在执行Task之前的初始化回调函数
             */
            void setThreadInitCallBack(const Task& tb){
                threadInitCallBack_ = tb;
            }
            void start(int numThreads);
            void stop();
            const std::string& getName()const{
                return name_;
            }
            size_t getQueueSize()const;
            void runTask(Task task);
            ~ThreadPool();

        private:
            void runInThread();
            Task take();
            bool isFull()const;
            mutable MutexLock mutex_;
            Condition notEmpty_;
            Condition notFull_;
            std::vector<std::unique_ptr<muduo::Thread>>threads_;
            size_t maxQueueSize;
            std::deque<Task>queue_;
            std::string name_;
            bool running_;
            Task threadInitCallBack_;
    };
}
#endif //MINI_MUDUO_BASE_THREADPOLL_H_
