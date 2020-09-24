//
// Created by zrzzzlll on 2020/8/29.
//

#ifndef MINI_MUDUO_NET_INCLUDE_CALLBACKS_H_
#define MINI_MUDUO_NET_INCLUDE_CALLBACKS_H_
#include <functional>
#include "Timestamp.h"
namespace muduo{
    namespace net{
        class Buffer;
        class TcpConnection;
        using TimerCallBack = std::function<void()>;
        using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
        using ConnectionCallback = std::function<void(const TcpConnectionPtr&)>;
        using MessageCallback = std::function<void(const TcpConnectionPtr&,Buffer*,
            Timestamp)>;
        using CloseCallBack = std::function<void(const TcpConnectionPtr&)>;
    }
}
#endif //MINI_MUDUO_NET_INCLUDE_CALLBACKS_H_
