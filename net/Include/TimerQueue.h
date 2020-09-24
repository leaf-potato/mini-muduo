//
// Created by zrzzzlll on 2020/4/3.
//

#ifndef MUDUO_NET_INCLUDE_TIMERQUEUE_H_
#define MUDUO_NET_INCLUDE_TIMERQUEUE_H_
#include "noncopyable.h"
#include "Timestamp.h"
#include <set>
#include <vector>
#include "Mutex.h"
#include "CallBacks.h"
#include "Channel.h"


namespace muduo{
    namespace net{
        class Timer;
        class EventLoop;
        class TimerId;
        /*
         * 该定时器是尽可能的保证准时
         * 不保证回调会按时进行
         */
        class TimerQueue:private noncopyable{

            public:
                explicit TimerQueue(EventLoop* loop);
                ~TimerQueue();
                //可以在其他线程被调用,必须是线程安全的
                TimerId addTimer(const TimerCallBack& cb,Timestamp when,double
                interval);

            private:
                using Entry = std::pair<Timestamp,Timer*>;
                using TimerList = std::set<Entry>;
                //定时器到期的回调函数
                void handleRead();
                //移除所有到期的定时器
                std::vector<Entry>getExpired(Timestamp now);
                void reset(const std::vector<Entry>&expired,Timestamp now);
                bool insert(Timer* timer);

                /*
                 * 私有属性
                 * timers_里面放的是所有的定时器
                 * loop_是属于哪个事件循环器
                 * timerfd_是定时器描述符
                 * timerfdChannel_是事件分发器
                 */
                TimerList timers_;
                EventLoop* loop_;
                const int timerfd_;
                Channel timerfdChannel_;

                /*
                 * 撤销定时器的一些属性
                 */
                bool callingExpiredTimers_;
        };
    }
}




#endif //MUDUO_NET_INCLUDE_TIMERQUEUE_H_
