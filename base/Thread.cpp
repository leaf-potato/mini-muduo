//
// Created by zrzzzlll on 2020/9/28.
//
#include "Thread.h"
#include "Exception.h"
#include "CurrentThread.h"
#include <unistd.h>
#include <sys/prctl.h>
#include <sys/syscall.h>
namespace muduo{
    namespace detail{
        pid_t getTid(){
            return static_cast<pid_t>(::syscall(SYS_gettid));
        }
        void afterFork(){
            CurrentThread::t_cachedTid = 0;
            CurrentThread::t_threadName = "main";
            CurrentThread::getTid();
        }
        class ThreadNameInitializer{
            public:
                ThreadNameInitializer(){
                    CurrentThread::t_threadName = "main";
                    CurrentThread::getTid();
                    pthread_atfork(nullptr, nullptr, &afterFork);
                }
        };
        ThreadNameInitializer init;
        class ThreadData{
            public:
                using ThreadFunc = muduo::Thread::ThreadFunc;
                ThreadData(ThreadFunc func,std::string& name,pid_t& tid,
                           CountDownLatch& latch):func_(std::move(func)),tid_(tid),
                           name_(name),latch_(latch){
                }
                void runInThread(){
                    /*
                     * 必须要等待线程完成之后才能进行初始化
                     */
                    tid_ = muduo::CurrentThread::getTid();
                    latch_.countDown();
                    muduo::CurrentThread::t_threadName = name_.empty() ?
                        "muduoThread" : name_.c_str();
                    prctl(PR_SET_NAME,muduo::CurrentThread::t_threadName);
                    try {
                        /*
                         * 该线程函数为用户设置的回调函数
                         * 在该函数中执行过程中可能会抛出异常
                         */
                        func_();
                        muduo::CurrentThread::t_threadName = "finished";
                    } catch (const Exception& ex) {
                        muduo::CurrentThread::t_threadName = "crashed";
                        fprintf(stderr, "exception caught in Thread %s\n", name_.c_str());
                        fprintf(stderr, "reason: %s\n", ex.what());
                        fprintf(stderr, "stack trace: %s\n", ex.stackTrace());
                        abort();
                    } catch (const std::exception& ex) {
                        muduo::CurrentThread::t_threadName = "crashed";
                        fprintf(stderr, "exception caught in Thread %s\n", name_.c_str());
                        fprintf(stderr, "reason: %s\n", ex.what());
                        abort();
                    } catch (...) {
                        muduo::CurrentThread::t_threadName = "crashed";
                        fprintf(stderr, "unknown exception caught in Thread %s\n", name_.c_str());
                        throw ;//再次抛异常
                    }
                }
            private:
                ThreadFunc func_;
                pid_t& tid_;
                std::string& name_;
                CountDownLatch& latch_;
        };
        void* startThread(void* obj){
            ThreadData data = *(static_cast<ThreadData*>(obj));
            data.runInThread();
            return nullptr;
        }
    }
    void Thread::setDefaultName() {
        int num = numCreated_.incrementAndGet();
        if(name_.empty()){
            char buf[32];
            snprintf(buf,sizeof(buf),"Thread%d",num);
            name_ = buf;
        }
    }
    AtomicInt32 Thread::numCreated_;
    Thread::Thread(ThreadFunc func, const std::string &name):
    started_(false),
    joined_(false),
    pthreadId_(0),
    tid_(0),
    func_(func),
    name_(name),
    latch_(1){
        setDefaultName();
    }
    void Thread::start() {
        assert(!started_);
        started_ = true;
        /*
         * 必须要调用pthread_create()函数创建线程之后
         * 才能通过gettid函数获取到线程的id，所以pthread_create()中
         * 不能直接执行ThreadFunc，必须对其进行进一步的封装
         */
        detail::ThreadData data(func_,name_,tid_,latch_);
        if(pthread_create(&pthreadId_, nullptr,&detail::startThread,&data)){
            started_ = false;
        }else{
            /*
             * 成功创建一个线程，但必须要等待新创建的线程将基本信息初始化完成
             */
            latch_.wait();
            assert(tid_ > 0);
        }
    }
    int Thread::join() {
        /*
         * pthread_join()函数以阻塞的方式等待一个线程的结束
         * 当pthread_join()函数返回的时候其指定的线程结束，线程资源被回收
         */
        assert(started_);
        assert(!joined_);
        joined_ = true;
        return pthread_join(pthreadId_, nullptr);
    }
    Thread::~Thread(){
        if(started_ && !joined_){
            pthread_detach(pthreadId_);
        }
    }
}