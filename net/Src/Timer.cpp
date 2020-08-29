//
// Created by zrzzzlll on 2020/4/3.
//

#include "Timer.h"

using namespace muduo;

void Timer::restart(Timestamp now) {
    if(repeat_){
        expiration_ =  addTime(now,interval_);
    }else{
        expiration_ = Timestamp::invalid();
    }
}
