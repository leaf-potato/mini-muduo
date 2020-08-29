//
// Created by zrzzzlll on 2020/4/3.
//

#ifndef MUDUO_NET_INCLUDE_TIMER_H_
#define MUDUO_NET_INCLUDE_TIMER_H_

#include "noncopyable.h"
#include "Timestamp.h"
#include "TimerCallBack.h"
namespace muduo{
    class Timer:private noncopyable{
        public:
            Timer(const TimerCallBack& cb,Timestamp when,double interval)
            :callback_(cb),
            expiration_(when),
            interval_(interval),
            repeat_(interval> 0){}

            void run()const {
                callback_();
            }
            Timestamp expiration()const{
                return expiration_;
            }
            bool repeat()const{
                return repeat_;
            }
            void restart(Timestamp now);
        private:
            const TimerCallBack callback_;
            Timestamp expiration_;
            const double interval_;
            const bool repeat_;

    };
}
#endif //MUDUO_NET_INCLUDE_TIMER_H_
