//
// Created by zrzzzlll on 2020/10/7.
//

#ifndef MINI_MUDUO_BASE_TIMESTAMP_H_
#define MINI_MUDUO_BASE_TIMESTAMP_H_
#include "copyable.h"
#include <sys/types.h>
#include <string>
namespace muduo{
    class Timestamp : public copyable{
        public:
            Timestamp():microSecondsSinceEpoch_(0){
            }
            explicit Timestamp(int64_t microSecondSinceEpoch):microSecondsSinceEpoch_
            (microSecondSinceEpoch){
            }

            void swap(Timestamp& that){
                std::swap(microSecondsSinceEpoch_,that.microSecondsSinceEpoch_);
            }

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

            static Timestamp getTimestampFromUnixTime(time_t time){
                return getTimestampFromUnixTime(time,0);
            }
            static Timestamp getTimestampFromUnixTime(time_t time ,int microSeconds){
                return Timestamp(static_cast<int64_t>(time) * kMicroSecondsPerSecond +
                microSeconds);
            }

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
