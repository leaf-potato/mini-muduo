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
#include "TimerCallBack.h"
#include "Channel.h"


namespace muduo{
    class Timer;
    class EventLoop;
    class TimerId;


    class TimerQueue:private noncopyable{

        public:
            TimerQueue(EventLoop* loop);
            ~TimerQueue();

        private:
            using Entry = std::pair<Timestamp,Timer*>;
            using TimerList = std::set<Entry>;
            void handleRead();
            std::vector<Entry>getExpired(Timestamp now);
            void reset(const std::vector<Entry>&expired,Timestamp now);
            bool insert(Timer* timer);



            TimerList timers_;
            EventLoop* loop_;
            const int timerfd_;
            Channel timerfdChannel_;
    };
}




#endif //MUDUO_NET_INCLUDE_TIMERQUEUE_H_
