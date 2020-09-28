//
// Created by zrzzzlll on 2020/9/28.
//

#ifndef MINI_MUDUO_BASE_ATOMIC_H_
#define MINI_MUDUO_BASE_ATOMIC_H_
#include "nocopyable.h"
#include <cstdint>
namespace muduo{
    namespace detail{
        template <typename T>
        class AtomicIntegerT:nocopyable{
            public:
                AtomicIntegerT():value_(0){}
                T get(){
                    /*
                     * Gcc提供的内置原子操作
                     * 函数原型：
                     * type __sync_val_compare_and_swap (type *ptr, type oldval type newval, ...)
                     * 返回操作之前*ptr的值
                     * 比较*ptr与oldval的值，如果两者相等，则将newval更新到*ptr
                     */
                    return __sync_val_compare_and_swap(&value_, 0, 0);
                }
                void set(T newValue){
                    getAndSet(newValue);
                }
                void add(T x){
                    addAndGet(x);
                }
                void reduce(T x){
                    reduceAndGet(x);
                }
                void increment(){
                    incrementAndGet();
                }
                void decrement(){
                    decrementAndGet();
                }

                T getAndAdd(T x){
                    /*
                     * 先获取value的值，再进行加号运算符
                     * 类似于后置++运算符
                     */
                    return __sync_fetch_and_add(&value_,x);
                }
                T addAndGet(T x){
                    /*
                     * 类似于前置++运算符
                     */
                    return getAndAdd(x) + x;
                }

                T getAndReduce(T x){
                    return getAndAdd(-x);
                }
                T reduceAndGet(T x){
                    return addAndGet(-x);
                }

                T getAndIncrement(){
                    return getAndAdd(1);
                }
                T incrementAndGet(){
                    return addAndGet(1);
                }

                T getAndDecrement(){
                    return getAndReduce(-1);
                }
                T decrementAndGet(){
                    return reduceAndGet(-1);
                }

                T getAndSet(T newValue){
                    /*
                     * 将value的值返回，并设置value为newvalue
                     */
                    return __sync_lock_test_and_set(&value_, newValue);
                }
                T setAndGet(T newValue){
                    return getAndSet(newValue) + newValue;
                }
            private:
                volatile T value_;
        };
    }
    using AtomicInt32 = detail::AtomicIntegerT<int32_t>;
    using AtomicInt64 = detail::AtomicIntegerT<int64_t>;
}



#endif //MINI_MUDUO_BASE_ATOMIC_H_
