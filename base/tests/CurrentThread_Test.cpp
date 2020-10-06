//
// Created by zrzzzlll on 2020/10/6.
//
#include "CurrentThread.h"
#include <iostream>
#include <unistd.h>
using namespace muduo;
int main(int argc,char* argv[]){
    std::cout << "pid = " << getpid() << std::endl;
    std::cout << "tid = " << CurrentThread::getTid() << std::endl;
    std::cout << "tidString = " << CurrentThread::getTidString() << std::endl;
    std::cout << "tidLength = " << CurrentThread::getTidStringLength() << std::endl;
    std::cout << "threadName = " << CurrentThread::getThreadName() << std::endl;
    std::cout << "isMainThread = " << std::boolalpha << CurrentThread::isMainThread() <<
    std::endl;
    return 0;
}