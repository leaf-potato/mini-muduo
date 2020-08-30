//
// Created by zrzzzlll on 2020/8/30.
//

#include "Acceptor.h"
#include "SocketsOps.h"
#include "EventLoop.h"
#include "InetAddress.h"
using namespace muduo;
using namespace muduo::net;
Acceptor::Acceptor(EventLoop *loop, const InetAddress &listenAddr)
:loop_(loop),acceptSocket_(sockets::createNonblockingOrDie(AF_INET)),acceptChannel_
(loop,acceptSocket_.fd()),listening_(false) {
    acceptSocket_.setReuseAddr(true);
    acceptSocket_.bindAddress(listenAddr);
    acceptChannel_.setReadCallBack(std::bind(&Acceptor::handleRead,this));
}
void Acceptor::listen() {
    loop_->assertInLoopThread();
    listening_ = true;
    acceptSocket_.listen();
    acceptChannel_.enableReading();
}
Acceptor::~Acceptor() {
    acceptChannel_.disableAll();
}
void Acceptor::handleRead() {
    loop_->assertInLoopThread();
    InetAddress peerAddr;
    int connfd = acceptSocket_.accept(&peerAddr);
    if(connfd >=0 ){
        if(newConnectionCallback_){
            newConnectionCallback_(connfd,peerAddr);
        }else{
            sockets::close(connfd);
        }
    }
}
