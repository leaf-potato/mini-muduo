//
// Created by zrzzzlll on 2020/10/7.
//
#include "Timestamp.h"
#include <sys/time.h>
#include <iostream>
#include <cassert>
using namespace muduo;
int main(int argc,char* argv[]){

    Timestamp now(Timestamp::getNowTimestamp());
    std::cout << "now time string is "<<now.toString() << std::endl;
    std::cout << "now formatted time with microSeconds is " << now.toFormattedString
    (true) << std::endl;
    std::cout << "now formatted time without microSeconds is " << now.toFormattedString
    (false) << std::endl;

    int64_t microSeconds = now.getMicroSecondsSinceEpoch();
    time_t seconds = now.getSecondsSinceEpoch();
    std::cout << "now time microSeconds = " << microSeconds << std::endl;
    std::cout <<"now time seconds = " << seconds << std::endl;

    Timestamp biggerTime = Timestamp::getTimestampFromUnixTime(now
        .getSecondsSinceEpoch() + 1);
    std::cout << "bigger time string is  "<< biggerTime.toString() << std::endl;
    Timestamp smallerTime = Timestamp::getTimestampFromUnixTime(now
        .getSecondsSinceEpoch() - 1);
    std::cout << "smaller time string is "<<smallerTime.toString() << std::endl;

    Timestamp copy(now);
    assert(copy == now);
    assert(biggerTime > now);
    assert(smallerTime < now);

    copy.swap(biggerTime);
    assert(copy == Timestamp::getTimestampFromUnixTime(now.getSecondsSinceEpoch() + 1));
    assert(biggerTime == now);

    Timestamp invalidTime = Timestamp::getInvalidTimestamp();
    assert(now.isValid());
    assert(!invalidTime.isValid());
    return 0;
}