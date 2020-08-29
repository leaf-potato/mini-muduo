//
// Created by zrzzzlll on 2020/4/3.
//

#ifndef MUDUO_NET_INCLUDE_TIMERID_H_
#define MUDUO_NET_INCLUDE_TIMERID_H_
#include "copyable.h"
namespace muduo{
    namespace net{
        class Timer;
        class TimerId:public copyable{
            public:
                TimerId():value_(nullptr),sequence_(0){}
                TimerId(Timer* timer,int64_t seq):value_(timer),sequence_(seq){}
                friend class TimerQueue;
            private:
                Timer* value_;
                int64_t sequence_;
        };
    }
}
#endif //MUDUO_NET_INCLUDE_TIMERID_H_
