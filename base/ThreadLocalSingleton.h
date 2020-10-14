//
// Created by zrzzzlll on 2020/10/14.
//

#ifndef MINI_MUDUO_BASE_THREADLOCALSINGLETON_H_
#define MINI_MUDUO_BASE_THREADLOCALSINGLETON_H_
#include "nocopyable.h"
#include <pthread.h>
#include <cassert>
namespace muduo{

    template<typename T>
    class ThreadLocalSingleton : public nocopyable{
        public:
            ThreadLocalSingleton() = delete;
            ~ThreadLocalSingleton() = delete;
            static T& getInstance(){
                if(!value_){
                    value_ = new T();
                    deleter_.set(value_);
                }
                return *value_;
            };
            static T* getPointer(){
                if(!value_){
                    value_ = new T();
                    deleter_.set(value_);
                }
                return value_;
            }
        private:
            static void destructor(void* obj)
            {
                assert(obj == value_);
                typedef char T_must_be_complete_type[sizeof(T) == 0 ? -1 : 1];
                T_must_be_complete_type dummy; (void) dummy;
                delete value_;
                value_ = nullptr;
            }
            class Deleter{
                public:
                    Deleter(){
                        pthread_key_create(&pkey_,ThreadLocalSingleton::destructor);
                    }
                    ~Deleter(){
                        pthread_key_delete(pkey_);
                    }
                    void set(T* newObj){
                        assert(pthread_getspecific(pkey_) == nullptr);
                        pthread_setspecific(pkey_,newObj);
                    }
                private:
                    pthread_key_t pkey_;
            };
            static __thread T* value_;
            static Deleter deleter_;
    };
    template<typename T>
    __thread T* ThreadLocalSingleton<T>::value_ = nullptr;
    template<typename T>
    typename ThreadLocalSingleton<T>::Deleter ThreadLocalSingleton<T>::deleter_;
}
#endif //MINI_MUDUO_BASE_THREADLOCALSINGLETON_H_
