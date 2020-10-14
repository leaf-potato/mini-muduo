//
// Created by zrzzzlll on 2020/10/14.
//

#ifndef MINI_MUDUO_BASE_SINGLETON_H_
#define MINI_MUDUO_BASE_SINGLETON_H_
#include "nocopyable.h"
#include <pthread.h>
#include <cassert>
#include <cstdlib>
namespace muduo{
    namespace detail{
            template<typename T>
            struct has_no_destroy{
                template<typename C>static char test(decltype(&C::no_destroy));
                template<typename C>static int32_t test(...);
                const static bool value = sizeof(test<T>(0)) == 1;
            };
    }
    template<typename T>
    class Singleton : public nocopyable{
        public:
            Singleton() = delete;
            ~Singleton() = delete;
            static T& getInstance(){
                pthread_once(&ponce_,&Singleton::init);
                assert(value_ != nullptr);
                return value_;
            }
        private:
            static void init(){
                value_ = new T();
                if(!detail::has_no_destroy<T>::value){
                    atexit(destroy);
                }
            }
            static void destroy(){
                typedef char T_must_be_complete_type[sizeof(T) == 0 ? -1 : 1];
                T_must_be_complete_type dummy; (void) dummy;
                delete value_;
                value_ = nullptr;
            }
            static pthread_once_t ponce_;
            static T* value_;
    };
    template<typename T>
    pthread_once_t Singleton<T>::ponce_ = PTHREAD_ONCE_INIT;
    template<typename T>
    T* Singleton<T>::value_ = nullptr;
}



#endif //MINI_MUDUO_BASE_SINGLETON_H_
