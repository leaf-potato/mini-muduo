//
// Created by zrzzzlll on 2020/8/30.
//
#include "Connection.h"
#include "EventLoop.h"
#include "Channel.h"
#include "Socket.h"
#include "Logging.h"
using namespace muduo::net;
void TcpConnection::handleClose() {
    loop_->assertInLoopThread();
    LOG_TRACE << "TcpConnection::handleClose state = "<< state_;
    assert(state_ == kConnected);
    channel_->disableAll();
    closeCallback_(shared_from_this());
}
TcpConnection::TcpConnection(EventLoop *loop,
                             const string &name,
                             int sockfd,
                             const InetAddress &localAddr,
                             const InetAddress &peerAddr):
                             loop_(loop),
                             name(name),
                             state_(kConnecting),
                             socket_(new Socket(sockfd)),
                             channel_(new Channel(loop,sockfd)),
                             localAddr_(localAddr),
                             peerAddr_(peerAddr){

}
