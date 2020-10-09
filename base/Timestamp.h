//
// Created by zrzzzlll on 2020/10/7.
//

#ifndef MINI_MUDUO_BASE_TIMESTAMP_H_
#define MINI_MUDUO_BASE_TIMESTAMP_H_
#include "copyable.h"
#include <sys/types.h>
#include <string>
namespace muduo{
    /*
     * 该类是可以复制的
     * 允许调用复制构造函数和赋值操作运算符
     * 在此处不需要重载相应的函数，采用编译器默认的函数即可
     */
    class Timestamp : public copyable{
        public:
            /*
             * 无参构造函数,有参构造函数
             */
            Timestamp():microSecondsSinceEpoch_(0){
            }
            explicit Timestamp(int64_t microSecondSinceEpoch):microSecondsSinceEpoch_
            (microSecondSinceEpoch){
            }

            void swap(Timestamp& that){
                std::swap(microSecondsSinceEpoch_,that.microSecondsSinceEpoch_);
            }
            /*
             * 将时间戳转换为字符串返回
             * 两个不同格式转换方式的函数
             */
            std::string toString()const;
            std::string toFormattedString(bool showMicroSeconds = true)const;

            bool isValid()const{
                return microSecondsSinceEpoch_ > 0;
            }

            int64_t getMicroSecondsSinceEpoch()const{
                return microSecondsSinceEpoch_;
            }
            time_t getSecondsSinceEpoch()const{
                return static_cast<time_t>(microSecondsSinceEpoch_ /
                kMicroSecondsPerSecond);
            }

            static const int kMicroSecondsPerSecond = 1000 * 1000;


            static Timestamp getNowTimestamp();
            static Timestamp getInvalidTimestamp();
            /*
             * 构造函数中的参数是以微妙为单位，类型为int64位整数
             * 这儿getTimestampFromUnixTime函数不采用重载构造函数替代的原因在于
             * time_t也是整数类型，这就会导致多重定义，所以这儿采用静态函数方式实现
             */
            static Timestamp getTimestampFromUnixTime(time_t time){
                return getTimestampFromUnixTime(time,0);
            }
            static Timestamp getTimestampFromUnixTime(time_t time ,int microSeconds){
                return Timestamp(static_cast<int64_t>(time) * kMicroSecondsPerSecond +
                microSeconds);
            }

            /*
             * 对相关操作运算符进行重载
             * 包括 < > <= >= == !=
             */
            bool operator < (const Timestamp& that)const{
                return microSecondsSinceEpoch_ < that.microSecondsSinceEpoch_;
            }
            bool operator >= (const Timestamp& that)const{
                return !(*this < that);
            }
            bool operator > (const Timestamp& that)const{
                return !(*this <= that);
            }
            bool operator <= (const Timestamp& that)const{
                return (*this < that) || (*this == that);
            }
            bool operator == (const Timestamp& that)const{
                return (*this >= that) && (that >= *this);
            }
            bool operator != (const Timestamp& that)const{
                return !(*this == that);
            }
        private:
            int64_t microSecondsSinceEpoch_;
    };
}
#endif //MINI_MUDUO_BASE_TIMESTAMP_H_
