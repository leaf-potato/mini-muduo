//
// Created by zrzzzlll on 2020/10/5.
//
#include "Mutex.h"
#include <syscall.h>
#include <unistd.h>
#include <pthread.h>
#include <vector>
#include <iostream>
using namespace muduo;
int global_value = 1;
MutexLock g_mutex;
pid_t getTid(){
    return static_cast<pid_t>(::syscall(SYS_gettid));
}
void* threadFunc(void * obj){
    for(int i = 0;i < 10;i++){
        MutexLockGuard lock(g_mutex);
        g_mutex.assignLocked();
        std::cout << "tid=" << getTid()
        << " print value " << global_value++ <<std::endl;
    }
    return nullptr;
}
int main(int argc,char* argv[]){
    int count_threads = 10;
    std::vector<pthread_t>threads;
    threads.reserve(count_threads);
    for(int i = 0;i < count_threads;i++){
        pthread_create(&threads[i], nullptr, &threadFunc, nullptr);
    }
    for(int i = 0;i < count_threads;i++){
        pthread_join(threads[i], nullptr);
    }
    return 0;
}