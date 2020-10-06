//
// Created by zrzzzlll on 2020/10/6.
//

#include "Atomic.h"
#include <cassert>
using namespace muduo;
int main(int argc,char* argv[]){

    AtomicInt32 number32;
    //对get方法的测试
    assert(number32.get() == 0);
    //对加法相关操作的测试
    assert(number32.getAndAdd(1) == 0);
    number32.add(3);
    assert(number32.get() == 4);
    assert(number32.addAndGet(1) == 5);
    //对减法相关操作的测试
    assert(number32.getAndReduce(2) == 5);
    number32.reduce(3);
    assert(number32.get() == 0);
    assert(number32.reduceAndGet(2) == -2);
    //对递增相关操作的测试
    assert(number32.getAndIncrement() == -2);
    number32.increment();
    assert(number32.get() == 0);
    assert(number32.incrementAndGet() == 1);
    //对递减相关操作的测试
    assert(number32.decrementAndGet() == 0);
    number32.decrement();
    assert(number32.get() == -1);
    assert(number32.getAndDecrement() == -1);
    //对重置相关操作的测试
    assert(number32.getAndSet(5) == -2);
    assert(number32.get() == 5);
    number32.set(10);
    assert(number32.get() == 10);


    AtomicInt32 number64;
    //对get方法的测试
    assert(number64.get() == 0);
    //对加法相关操作的测试
    assert(number64.getAndAdd(1) == 0);
    number64.add(3);
    assert(number64.get() == 4);
    assert(number64.addAndGet(1) == 5);
    //对减法相关操作的测试
    assert(number64.getAndReduce(2) == 5);
    number64.reduce(3);
    assert(number64.get() == 0);
    assert(number64.reduceAndGet(2) == -2);
    //对递增相关操作的测试
    assert(number64.getAndIncrement() == -2);
    number64.increment();
    assert(number64.get() == 0);
    assert(number64.incrementAndGet() == 1);
    //对递减相关操作的测试
    assert(number64.decrementAndGet() == 0);
    number64.decrement();
    assert(number64.get() == -1);
    assert(number64.getAndDecrement() == -1);
    //对重置相关操作的测试
    assert(number64.getAndSet(5) == -2);
    assert(number64.get() == 5);
    number64.set(10);
    assert(number64.get() == 10);
    return 0;
}