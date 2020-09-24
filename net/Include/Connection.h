//
// Created by zrzzzlll on 2020/8/30.
//

#ifndef MINI_MUDUO_NET_INCLUDE_CONNECTION_H_
#define MINI_MUDUO_NET_INCLUDE_CONNECTION_H_
#include "noncopyable.h"
#include <memory>
#include "CallBacks.h"
#include "InetAddress.h"
struct tcp_info;
namespace muduo{
    namespace net{
        class Channel;
        class EventLoop;
        class Socket;
        class TcpConnection: private noncopyable, public
            std::enable_shared_from_this<TcpConnection>{
            public:
                TcpConnection(EventLoop* loop,const string& name,int sockfd,const
                InetAddress& localAddr,const InetAddress& peerAddr);
                void setCloseCallback(const CloseCallBack& cb){closeCallback_ = cb;}
                void connectEstablished();
                void connectDestroyed();
            private:
                void handleRead();
                void handleWrite();
                void handleClose();
                void handleError();
                /*
                 * Connection的状态
                 */
                enum StateE{kConnecting,kConnected,kDisconnected};
                void setState(StateE state){state_ = state;}
                StateE state_;
                EventLoop* loop_;
                std::unique_ptr<Socket>socket_;
                std::unique_ptr<Channel>channel_;
                std::string name;
                InetAddress localAddr_;
                InetAddress peerAddr_;
                ConnectionCallback connectionCallback_;
                MessageCallback messageCallback_;
                CloseCallBack closeCallback_;
        };
    }
}
#endif //MINI_MUDUO_NET_INCLUDE_CONNECTION_H_
