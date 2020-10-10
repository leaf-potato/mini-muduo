//
// Created by zrzzzlll on 2020/10/10.
//
#include "Date.h"
#include <cassert>
#include <iostream>
using namespace muduo;
int main(int argc,char* argv[]){
    Date invalid;
    assert(invalid.isValid() == false);

    int year = 2020,month = 10,day = 10;
    Date today(year,month,day);
    std::cout << today.toString() << std::endl;
    assert(today.getYear() == year);
    assert(today.getMonth() == month);
    assert(today.getDay() == day);
    assert(today.getWeekDay() == 6);

    Date tomorrow(today.getJulianDayNumber() + 1);
    assert(tomorrow.getDay() == day + 1);
    assert(tomorrow.getWeekDay() == 0);

    std::cout << "All Passed" << std::endl;
    return 0;
}

