//
// Created by zrzzzlll on 2020/4/2.
//

#ifndef MUDUO_NET_INCLUDE_EVENTLOOP_H_
#define MUDUO_NET_INCLUDE_EVENTLOOP_H_
#include "CurrentThread.h"
#include "Thread.h"
#include "functional"
#include "CallBacks.h"
#include "Timestamp.h"
#include <vector>
#include <boost/scoped_ptr.hpp>
#include "noncopyable.h"
#include "TimerId.h"
namespace muduo{
    namespace net{
        /*
        * 前置声明Channel和Poller
        */
        class Channel;
        class TimerQueue;
        class Poller;

        /*
         * 保证Eventloop一个线程只有一个实例
         * 同时使用Eventloop对象有些函数不能跨线程使用
         */
        class EventLoop: private noncopyable{
            public:
                using Functor = std::function<void()>;
                EventLoop();
                ~EventLoop();
                void loop();
                void assertInLoopThread(){
                    if(!isInLoopThread()){
                        abortNotInLoopThread();
                    }
                }
                //判断是否在IO线程中
                bool isInLoopThread()const{
                    return this->threadId_ == CurrentThread::tid();
                }
                static EventLoop* getEventLoopOfCurrentThread();
                void quit();
                void updateChannel(Channel* channel);
                /*
                 * 三个函数都是线程安全的，可以在其他线程(非IO线程)中被调用
                 */
                TimerId runAt(const Timestamp& time,const TimerCallBack& cb);
                TimerId runAfter(double delay,const TimerCallBack& cb);
                TimerId runEvery(double interval,const TimerCallBack& cb);
                /*
                 * 其他线程调用在IO线程中执行相关函数
                 * 线程安全的，可以在其他线程中调用
                 * runInLoop与queueInLoop的区别：
                 * 1. runInLoop不管是IO线程还是其他线程调用，都会立刻去执行callback
                 * 2. queueInLoop函数如果是在非IO线程中调用会立刻执行
                 *    如果是在IO线程中调用会在事件分发结束之后调用
                 */
                void runInLoop(const Functor& cb);
                void queueInLoop(const Functor& cb);
            private:
                void wakeUp();
                void doPendingFunctors();
                void abortNotInLoopThread();
                //wakeupFd_可读时的事件处理函数
                void handleRead();
                using ChannelList = std::vector<Channel*>;
                bool looping_;
                bool quit_;
                /*
                 * 记录构造对象的线程ID
                 */
                const pid_t threadId_;
                /*
                 * 通过一个unique_ptr间接引用一个Poller和TimerQueue;
                 */
                std::unique_ptr<Poller>poller_;
                std::unique_ptr<TimerQueue>timerQueue_;
                ChannelList activeChannels_;
                /*
                 * 唤醒主线程相关属性
                 */
                mutable MutexLock mutex_;
                int wakeupFd_;
                std::vector<Functor>pendingFunctors_;
                bool callingPendingFunctors_;//atomic
        };
    }
}

#endif //MUDUO_NET_INCLUDE_EVENTLOOP_H_
