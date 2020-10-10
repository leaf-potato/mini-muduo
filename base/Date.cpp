//
// Created by zrzzzlll on 2020/10/10.
//

#include "Date.h"
namespace muduo{
    namespace detail{
        char require_32_bit_integer_at_least[sizeof(int) >= sizeof(int32_t) ? 1 : -1];
        int getJulianDayNumber(int year,int month,int day){
            (void)require_32_bit_integer_at_least;
            int a = (14 - month) / 12;
            int y = year + 4800 - a;
            int m = month + 12 * a - 3;
            return day + (153*m + 2) / 5 + y*365 + y/4 - y/100 + y/400 - 32045;
        }
        Date::YearMonthDay getYearMonthDay(int julianDayNumber){
            int a = julianDayNumber + 32044;
            int b = (4 * a + 3) / 146097;
            int c = a - ((b * 146097) / 4);
            int d = (4 * c + 3) / 1461;
            int e = c - ((1461 * d) / 4);
            int m = (5 * e + 2) / 153;
            Date::YearMonthDay ymd;
            ymd.day = e - ((153 * m + 2) / 5) + 1;
            ymd.month = m + 3 - 12 * (m / 10);
            ymd.year = b * 100 + d - 4800 + (m / 10);
            return ymd;
        }
    }
    const int Date::kJulianDayOf1970_01_01 = detail::getJulianDayNumber(1970,1,1);
}

using namespace muduo;
using namespace detail;
std::string Date::toString() const {
    YearMonthDay date = getYearMonthDay();
    char buf[32] = {0};
    snprintf(buf,sizeof(buf),"%04d-%02d-%02d",date.year,date.month,date.day);
    return buf;
}
Date::Date(int year, int month, int day):julianDayNumber_(detail::getJulianDayNumber
(year,month,day)) {
}

Date::Date(const struct tm& date):julianDayNumber_(detail::getJulianDayNumber(date
    .tm_year + 1900,date.tm_mon + 1,date.tm_mday)) {
}

Date::YearMonthDay Date::getYearMonthDay() const {
    return ::getYearMonthDay(julianDayNumber_);
}