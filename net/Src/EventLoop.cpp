//
// Created by zrzzzlll on 2020/4/2.
//

#include "EventLoop.h"
#include "Channel.h"
#include "Poller.h"
#include <cassert>
#include "Logging.h"


using namespace muduo::net;
/*
 * __thread每个线程有一份独立的实体，各个线程的值互不干扰
 * 可以用来修饰那些带有全局性且值可能变，但是又不值得用全局变量保护的变量
 */
namespace {
    __thread EventLoop* t_loopInThisThread = nullptr;
    const int kPollTimeMs = 10000;
}
/*
 * Eventloop类的构造函数
 * 初始化looping为false，threadId为当前进程ID
 */
EventLoop::EventLoop(): looping_(false),
                        quit_(false), threadId_(CurrentThread::tid()), poller_(new Poller(this)){
    /*
     * 创建一个对象打日志
     */
    LOG_TRACE << "EventLoop created" << this << " in thread " << threadId_;
    //如果当前线程已经创建了一个EventLoop的话，那么就直接报错退出
    if(t_loopInThisThread){
        LOG_FATAL << "Another EventLoop "
        << t_loopInThisThread << " exists in this thread " << threadId_;
    }else{
        t_loopInThisThread = this;
    }
}
EventLoop::~EventLoop() {
    assert(!looping_);
    t_loopInThisThread = nullptr;
}
void EventLoop::abortNotInLoopThread() {
    LOG_FATAL << "EventLoop::abortNotInLoopThread - EventLoop" << this
              << "was created in threadID_ " << threadId_
              << ", current thread id is " << CurrentThread::tid();
}
void EventLoop::quit() {
    //可以跨线程调用
    quit_ = true;
}
void EventLoop::updateChannel(Channel *channel) {
    assert(channel->ownerLoop() == this);
    assertInLoopThread();
    poller_->updateChannel(channel);
}
void EventLoop::loop() {
    //事件循环必须在IO线程中，同时该EventLoop没有启动循环
    //该函数只能在IO线程中调用，所以使用到的变量都无须加锁
    assert(!looping_);
    assertInLoopThread();
    looping_ = true;
    quit_ = false;

    while(!quit_){
        activeChannels_.clear();
        poller_->poll(kPollTimeMs,&activeChannels_);
        for(ChannelList::iterator it = activeChannels_.begin();
        it != activeChannels_.end();it++){
            (*it)->handleEvent();
        }
        doPendingFunctors();
    }
    LOG_TRACE << "Eventloop "<< this << " stop looping";
    looping_ = false;
}
EventLoop *EventLoop::getEventLoopOfCurrentThread() {
    return t_loopInThisThread;
}
void EventLoop::runInLoop(const EventLoop::Functor &cb) {
    if(isInLoopThread()){
        cb();
    }else{
        queueInLoop(cb);
    }
}
void EventLoop::queueInLoop(const EventLoop::Functor &cb) {
    MutexLockGuard lock(mutex_);
    pendingFunctors_.push_back(cb);
    if(!isInLoopThread() || callingPendingFunctors_)
        wakeUp();
}
void EventLoop::doPendingFunctors() {
    std::vector<Functor>functors;
    callingPendingFunctors_ = true;
    {
        /*
         * 缩减临界区
         * 同时为了避免Functor调用queueInLoop而造成死锁
         */
        MutexLockGuard lock(mutex_);
        functors.swap(pendingFunctors_);
    }
    for(size_t i = 0;i < functors.size();i++){
        functors[i]();
    }
    callingPendingFunctors_ = false;
}
