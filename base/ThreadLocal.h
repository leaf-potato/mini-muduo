//
// Created by zrzzzlll on 2020/10/12.
//

#ifndef MINI_MUDUO_BASE_THREADLOCAL_H_
#define MINI_MUDUO_BASE_THREADLOCAL_H_
#include "nocopyable.h"
#include <pthread.h>
/*
 * 全局变量，但每个线程有自己单独的存储值
 */
namespace muduo{
    /*
     * ThreadLocal class是一个模板类，使用该模板类可以创建任意类型的线程存储对象
     * 使用方法如下：
     * ThreadLocal<string>ThreadName;创建一个string类型的线程存储对象
     * ThreadName表示线程的名字，每个线程中其值不同
     * ThreadName.getValue();获取线程存储对象的值
     */
    template<typename T>
    class ThreadLocal : public nocopyable{
        public:
            ThreadLocal(){
                /*
                 * 调用pthread_key_create()之后会新创建一个线程存储，相当
                 * 于在线程的指针数组中新分配一个槽      
                 * 同时将其通过pthread_getspecific获取的值初始化为nullptr。
                 */
                pthread_key_create(&pkey_,&ThreadLocal::destructor);
            }
            ~ThreadLocal(){
                /*
                 * 调用pthread_key_delete()函数将pkey_对应槽位内存空间释放
                 * 在释放的同时会去调用destructor函数
                 */
                pthread_key_delete(pkey_);
            }
            T& getValue(){
                T* perThreadValue = static_cast<T*>(pthread_getspecific(pkey_));
                if(!perThreadValue){
                    T* newObj = new T();
                    pthread_setspecific(pkey_,newObj);
                    perThreadValue = newObj;
                }
                return *perThreadValue;
            }
        private:
            /*
             * 该函数没有涉及访问对象的成员变量,应该被所有对象共享
             * 该函数的作用类似为类中的析构函数
             * 当调用pthread_delete()函数销毁pthread_key_t类型变量时，该函数
             * 会被调用，形参x的值为pthread_getspecific函数获取出的指针值，目的
             * 是为了能够释放动态分配的内存
             */
            static void destructor(void* x){
                T* obj = static_cast<T*>(x);
                /*
                 * 检测一下T是否是一个完整的类型（非前置声明）
                 * 前置声明的类型是不能对其进行运算操作包括sizeof,delete和new运
                 * 算符，否则会报错。所以这儿对T求sizeof并命别名
                 */
                typedef char T_must_be_complete_type[sizeof(T) == 0 ? -1 : 1];
                /*
                 * 在开启编译警告的情况下(-Wall)
                 * 如果在函数内部使用typedef定义了一个局部的别名且没有使用过
                 * 编译器会发出-Wunused-local-typedefs警告
                 * 所以这儿使用T_must_be_complete_type定义一个变量来消除警告
                 */
                T_must_be_complete_type dummy;
                /*
                 * 在开启编译警告的情况下(-Wall)
                 * 如果定义了一个局部变量或者静态变量但未使用过
                 * 编译器会发出-Wunused-variable警告
                 * 所以这儿使用(void)dummy消除警告
                 */
                (void)dummy;
                delete obj;
            }
            /*
             * pthread_key_t线程存储与具体存储对象的类型无关
             * 其pthread_getspecific和pthread_setspecific函数设置和获取的都
             * 是void类型的指针，所以可以实现任意类型的线程存储。
             */
            pthread_key_t pkey_;
    };
}
#endif //MINI_MUDUO_BASE_THREADLOCAL_H_
