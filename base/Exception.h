//
// Created by zrzzzlll on 2020/9/29.
//

#ifndef MINI_MUDUO_BASE_EXCEPTION_H_
#define MINI_MUDUO_BASE_EXCEPTION_H_
#include <exception>
#include <string>
namespace muduo{
    class Exception : public std::exception{
        public:
            Exception(const std::string& msg);
            ~Exception()noexcept override = default;
            const char* what()const noexcept override{
                return message_.c_str();
            }
            const char* stackTrace()const noexcept{
                return stack_.c_str();
            }
        private:
            std::string message_;
            std::string stack_;
    };
}
#endif //MINI_MUDUO_BASE_EXCEPTION_H_
