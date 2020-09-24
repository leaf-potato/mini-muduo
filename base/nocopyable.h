//
// Created by zrzzzlll on 2020/9/24.
//

#ifndef MINI_MUDUO_BASE_NOCOPYABLE_H_
#define MINI_MUDUO_BASE_NOCOPYABLE_H_
/*
 * 这是一个不可实例化的类
 * 继承该基类的类不可以进行赋值和复制操作
 */
namespace muduo{
    class nocopyable{
        public:
            nocopyable(const nocopyable&) = delete;
            nocopyable& operator=(const nocopyable&) = delete;
        protected:
            nocopyable() = default;
            ~nocopyable() = default;
    };
}
#endif //MINI_MUDUO_BASE_NOCOPYABLE_H_
