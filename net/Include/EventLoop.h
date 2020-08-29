//
// Created by zrzzzlll on 2020/4/2.
//

#ifndef MUDUO_NET_INCLUDE_EVENTLOOP_H_
#define MUDUO_NET_INCLUDE_EVENTLOOP_H_
#include "CurrentThread.h"
#include "Thread.h"
#include <vector>
#include <boost/scoped_ptr.hpp>
#include "noncopyable.h"
namespace muduo{
    namespace net{
        /*
        * 前置声明Channel和Poller
        */
        class Channel;
        class Poller;
        /*
         * 保证Eventloop一个线程只有一个实例
         * 同时使用Eventloop对象有些函数不能跨线程使用
         */
        class EventLoop: private noncopyable{
            public:
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
            private:
                void abortNotInLoopThread();
                using ChannelList = std::vector<Channel*>;
                bool looping_;
                bool quit_;
                /*
                 * 记录构造对象的线程ID
                 */
                const pid_t threadId_;
                /*
                 * 通过一个scoped_ptr间接引用一个poll
                 */
                boost::scoped_ptr<Poller>poller_;
                ChannelList activeChannels_;
        };
    }
}

#endif //MUDUO_NET_INCLUDE_EVENTLOOP_H_
