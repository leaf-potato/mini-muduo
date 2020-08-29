//
// Created by zrzzzlll on 2020/4/2.
//

#include "EventLoop.h"
#include "Channel.h"
#include "Logging.h"
#include <poll.h>
using namespace muduo::net;
/*
 * 事件的常量值
 */
const int Channel::kNoneEvent = 0;
const int Channel::kReadEvent = POLLIN | POLLPRI;
const int Channel::kWriteEvent = POLLOUT;

Channel::Channel(EventLoop *loop, int fd)
:loop_(loop),fd_(fd),events_(0),revents_(0),index_(-1) {
}

void Channel::update() {
    /* 调用其所属loop的updateChannel函数
     * 其所属的loop会调用间接拥有Poller的updateChannel函数
     */
    loop_->updateChannel(this);
}

void Channel::handleEvent() {
    /*
     * 根据revents_的值调用不同的回调函数
     */
    if(revents_ & POLLNVAL){
        LOG_WARN << "Channel:handleEvent() POLLNVAL";
    }
    if(revents_ & (POLLERR | POLLNVAL)){
        if(errorCallBack_) errorCallBack_;
    }
    if(revents_ & (POLLIN | POLLPRI | POLLRDHUP)){
        if(readCallBack_) readCallBack_;
    }
    if(revents_ & POLLOUT){
        if(writeCallBack_) writeCallBack_;
    }
}


