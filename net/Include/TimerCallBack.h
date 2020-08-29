//
// Created by zrzzzlll on 2020/4/3.
//

#ifndef MUDUO_NET_INCLUDE_TIMERCALLBACK_H_
#define MUDUO_NET_INCLUDE_TIMERCALLBACK_H_

#include "Timestamp.h"
#include <functional>
#include <memory>

namespace muduo{
    using TimerCallBack = std::function<void()>;
}



#endif //MUDUO_NET_INCLUDE_TIMERCALLBACK_H_
