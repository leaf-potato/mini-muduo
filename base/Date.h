//
// Created by zrzzzlll on 2020/10/10.
//

#ifndef MINI_MUDUO_BASE_DATE_H_
#define MINI_MUDUO_BASE_DATE_H_
#include "copyable.h"
#include <string>
struct tm;
namespace muduo{
    class Date : public copyable{
        public:
            using YearMonthDay = struct YearMonthDay{
                int year;
                int month;
                int day;
            };
            //构造函数默认构造一个无效日期
            Date():julianDayNumber_(0){
            }
            Date(int year,int month,int day);
            explicit Date(const struct tm&);
            explicit Date(int julianDayNumber):julianDayNumber_(julianDayNumber){
            }

            void swap(Date& date){
                std::swap(julianDayNumber_,date.julianDayNumber_);
            }
            std::string toString()const;
            bool isValid()const{
                return julianDayNumber_ > 0;
            }

            YearMonthDay getYearMonthDay()const;
            int getYear()const{
                return getYearMonthDay().year;
            }
            int getMonth()const{
                return getYearMonthDay().month;
            }
            int getDay()const{
                return getYearMonthDay().day;
            }
            // [0, 1, ..., 6] => [Sunday, Monday, ..., Saturday ]
            int getWeekDay()const{
                return (julianDayNumber_ + 1) % kDaysPeerWeek;
            }
            int getJulianDayNumber()const{
                return julianDayNumber_;
            }
        private:
            static const int kDaysPeerWeek = 7;
            static const int kJulianDayOf1970_01_01;
            /*
             * int类型整数为32个字节，最大可以表示2147483647
             * 按照一年365天来计算，可以表示约5883516年
             */
            int julianDayNumber_;
    };
}
#endif //MINI_MUDUO_BASE_DATE_H_
