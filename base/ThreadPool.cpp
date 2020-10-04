//
// Created by zrzzzlll on 2020/10/4.
//

#include "ThreadPool.h"
#include "Exception.h"
using namespace muduo;
ThreadPool::ThreadPool(const std::string &nameArg)
    :mutex_(),
    notEmpty_(mutex_),
    notFull_(mutex_),
    name_(nameArg),
    maxQueueSize(0),
    running_(false){
}

ThreadPool::~ThreadPool() {
    if(running_){
        stop();
    }
}
/*
 * 该函数只能被一个线程调用，所以不需要加锁
 * 同时只能被调用一次
 */
void ThreadPool::start(int numThreads) {
    assert(threads_.empty());
    running_ = true;
    threads_.reserve(numThreads);
    for(int i = 0;i < numThreads;i++){
        char id[32];
        snprintf(id,sizeof(id),"%d",i + 1);
        threads_.emplace_back(new muduo::Thread(std::bind(&ThreadPool::runInThread,
                                                          this),name_ + id));
        threads_[i]->start();
    }
}
void ThreadPool::stop() {
    {
        MutexLockGuard lock(mutex_);
        running_ = false;
        notEmpty_.notifyAll();
        notFull_.notifyAll();
    }
    for(auto& thr:threads_){
        thr->join();
    }
}
size_t ThreadPool::getQueueSize() const {
    MutexLockGuard lock(mutex_);
    return queue_.size();
}


bool ThreadPool::isFull() const {
    mutex_.assignLocked();
    return maxQueueSize > 0 && queue_.size() >= maxQueueSize;
}

void ThreadPool::runTask(Task task) {
    if(threads_.empty()){
        task();
    }else{
        MutexLockGuard lock(mutex_);
        while(isFull() && running_){
            notFull_.wait();
        }
        if(!running_)return ;
        assert(!isFull());
        queue_.push_back(std::move(task));
        notEmpty_.notify();
    }
}
ThreadPool::Task ThreadPool::take() {
    MutexLockGuard lock(mutex_);
    while(queue_.empty() && running_){
        notEmpty_.wait();
    }
    Task task;
    if(!queue_.empty()){
        task = queue_.front();
        queue_.pop_front();
        if(maxQueueSize > 0){
            notFull_.notify();
        }
    }
    return task;
}


void ThreadPool::runInThread() {
    try {
        if(threadInitCallBack_){
            threadInitCallBack_();
        }
        while(running_){
            Task task(take());
            if(task){
                task();
            }
        }
    } catch (Exception &ex) {
        fprintf(stderr, "exception caught in ThreadPool %s\n", name_.c_str());
        fprintf(stderr, "reason: %s\n", ex.what());
        fprintf(stderr, "stack trace: %s\n", ex.stackTrace());
        abort();
    }catch (const std::exception& ex){
        fprintf(stderr, "exception caught in ThreadPool %s\n", name_.c_str());
        fprintf(stderr, "reason: %s\n", ex.what());
        abort();
    }catch (...)
    {
        fprintf(stderr, "unknown exception caught in ThreadPool %s\n", name_.c_str());
        throw; // rethrow
    }
}