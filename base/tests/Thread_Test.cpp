//
// Created by zrzzzlll on 2020/10/5.
//
#include "Thread.h"
#include "CurrentThread.h"
#include "CountDownLatch.h"
#include <iostream>
#include <functional>
using namespace muduo;

CountDownLatch g_latch(1);
CountDownLatch latch(1);
void mysleep(int seconds){
    timespec t = { seconds, 0 };
    nanosleep(&t, NULL);
}
void threadFunc(){
    std::cout << "threadName = " << CurrentThread::getThreadName() << std::endl;
    std::cout << "tid = " << CurrentThread::getTid() << std::endl;
}

void threadFunc2(int x){
    std::cout << "threadName = " << CurrentThread::getThreadName() << std::endl;
    std::cout << "tid = " << CurrentThread::getTid() << "  x = " << x << std::endl;
}

void threadFunc3(){
    g_latch.wait();
    std::cout << "threadName = " << CurrentThread::getThreadName() << std::endl;
    std::cout << "tid = " << CurrentThread::getTid() << std::endl;
};
void threadFunc4(){
    std::cout << "threadName = " << CurrentThread::getThreadName() << std::endl;
    std::cout << "tid = " << CurrentThread::getTid() << std::endl;
    latch.countDown();
};
class Foo{
    public:
        explicit Foo(int x):x_(x){
        }
        void printText()const{
            std::cout << "threadName = " << CurrentThread::getThreadName() << std::endl;
            std::cout << "tid = " << CurrentThread::getTid() << std::endl;
            std::cout << "private value = " << x_ << std::endl;
        }
        void printText(const std::string& text)const{
            std::cout << "threadName = " << CurrentThread::getThreadName() << std::endl;
            std::cout << "tid = " << CurrentThread::getTid() << std::endl;
            std::cout << "private value = " << x_ << std::endl;
            std::cout << "text content = " << text << std::endl;
        }
    private:
        int x_;
};


int main(int argc,char* argv[]){
    std::cout << "threadName = " << CurrentThread::getThreadName() << std::endl;
    //判断进程id是否和主线程的id相同
    std::cout << "pid = " << gettid() << " tid = " << CurrentThread::getTid() <<
    std::endl;
    std::cout << std::endl;

    //构造函数只传入一个回调函数，采用默认的名字
    Thread t1(threadFunc);
    t1.start();
    t1.join();
    std::cout << "t1.tid = " << t1.getTid() << std::endl;
    std::cout << "t1.threadName = " << t1.getThreadName() << std::endl;
    std::cout << std::endl;

    Thread t2(std::bind(threadFunc2,42),
              "thread for free function with argument");
    t2.start();
    t2.join();
    std::cout << "t2.tid = " << t2.getTid() << std::endl;
    std::cout << "t2.threadName = " << t2.getThreadName() << std::endl;
    std::cout << std::endl;

    Foo foo(400);
    Thread t3(std::bind((void(Foo::*)()const)&Foo::printText,&foo),
              "thread for member function without argument");
    t3.start();
    t3.join();
    std::cout << std::endl;

    Thread t4(std::bind((void(Foo::*)(const std::string&)const)&Foo::printText,&foo,
                        "zhaoran"));
    t4.start();
    t4.join();
    std::cout << "t4.tid = " << t4.getTid() << std::endl;
    std::cout << "t4.threadName = " << t4.getThreadName() << std::endl;
    std::cout << std::endl;

    {
        Thread t5(threadFunc3);
        t5.start();
    }
    g_latch.countDown();
    mysleep(2);
    std::cout << std::endl;

    {
        Thread t6(threadFunc4);
        t6.start();
        latch.wait();
    }
    mysleep(2);
    std::cout << std::endl;
    std::cout << "创建线程的数量为：" << Thread::getThreadCreatedNumber() << std::endl;
    return 0;
}

