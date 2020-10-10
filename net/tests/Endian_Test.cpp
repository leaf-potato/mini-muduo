//
// Created by zrzzzlll on 2020/10/10.
//
#include "Endian.h"
#include <iostream>
using namespace muduo;
using namespace net;
using namespace sockets;
int main(int argc,char* argv[]){

    union {
        uint32_t u32;
        uint8_t arr[4];
    }var;
    var.arr[0] = 0x11;
    var.arr[1] = 0x22;
    var.arr[2] = 0x33;
    var.arr[3] = 0x44;
    std::cout << std::hex << var.u32 << std::endl;
    std::cout << std::hex << hostToNetwork32(var.u32) << std::endl;
    return 0;
}