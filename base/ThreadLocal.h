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
    template<typename T>
    class ThreadLocal : public nocopyable{
        public:
            ThreadLocal(){
                pthread_key_create(&pkey_,&ThreadLocal::destructor);
            }
            ~ThreadLocal(){
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
            static void destructor(void* x){
                T* obj = static_cast<T*>(x);
                typedef char T_must_be_complete_type[sizeof(T) == 0 ? -1 : 1];
                T_must_be_complete_type dummy;
                (void)dummy;
                delete obj;
            }
            pthread_key_t pkey_;
    };
}
#endif //MINI_MUDUO_BASE_THREADLOCAL_H_
