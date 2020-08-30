//
// Created by zrzzzlll on 2020/8/30.
//

#ifndef MINI_MUDUO_NET_INCLUDE_ACCEPTOR_H_
#define MINI_MUDUO_NET_INCLUDE_ACCEPTOR_H_
#include "Channel.h"
#include "Socket.h"
#include "functional"
namespace muduo{
    namespace net{
        class EventLoop;
        class InetAddress;
        class Acceptor: private noncopyable{
            public:
                using NewConnectionCallback = std::function<void(int sockfd,const
                InetAddress&)>;
                Acceptor(EventLoop* loop,const InetAddress& listenAddr);
                ~Acceptor();
                void setNewConnectionCallback(const NewConnectionCallback& cb){
                    newConnectionCallback_ = cb;
                }
                void listen();
                bool listening()const{return listening_;}
            private:
                void handleRead();
                EventLoop* loop_;
                Socket acceptSocket_;
                Channel acceptChannel_;
                NewConnectionCallback newConnectionCallback_;
                bool listening_;
        };
    }
}
#endif //MINI_MUDUO_NET_INCLUDE_ACCEPTOR_H_
