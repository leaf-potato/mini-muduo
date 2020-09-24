//
// Created by zrzzzlll on 2020/4/3.
//

#include "TimerQueue.h"

#include "EventLoop.h"
#include "Logging.h"
#include "Timer.h"
#include "TimerId.h"
#include <functional>
#include <sys/timerfd.h>

namespace muduo{
    namespace net{
        namespace detail{
            int createTimerfd(){
                /*
                 * 函数原型:timer_create(int clock_id,int flags)
                 */
                int timerfd = ::timerfd_create(CLOCK_MONOTONIC,
                                               TFD_NONBLOCK |TFD_CLOEXEC);
                if(timerfd < 0)
                    LOG_SYSFATAL << "Failed in timerfd_create";
                return timerfd;
            }
        }
    }
}
using namespace muduo;
using namespace muduo::net;
using namespace muduo::net::detail;

TimerQueue::TimerQueue(EventLoop *loop):loop_(loop),
                                        timerfd_(createTimerfd()),
                                        timerfdChannel_(loop,timerfd_),
                                        timers_(),
                                        callingExpiredTimers_(false){
    timerfdChannel_.setReadCallBack(std::bind(&TimerQueue::handleRead,this));
    timerfdChannel_.enableReading();
}
TimerQueue::~TimerQueue() {
    timerfdChannel_.disableAll();
    ::close(timerfd_);
    for(const Entry& entry:timers_){
        delete entry.second;
    }
}
TimerId TimerQueue::addTimer(const TimerCallBack &cb, Timestamp when, double interval) {


    return TimerId();
}



