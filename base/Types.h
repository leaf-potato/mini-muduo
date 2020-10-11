//
// Created by zrzzzlll on 2020/10/10.
//

#ifndef MINI_MUDUO_BASE_TYPES_H_
#define MINI_MUDUO_BASE_TYPES_H_
namespace muduo{
    template <typename To,typename From>
    inline To implicit_cast(const From& f){
        return f;
    }
}
#endif //MINI_MUDUO_BASE_TYPES_H_
