//
// Created by zrzzzlll on 2020/4/3.
// 对IO multiplexing的封装
//

#ifndef MUDUO_NET_INCLUDE_POLLER_H_
#define MUDUO_NET_INCLUDE_POLLER_H_

#include <map>
#include <vector>
#include "EventLoop.h"
#include "Timestamp.h"
struct pollfd;

namespace muduo{
    namespace net{
        class Channel;
        /*
         * EventLoop的间接成员
         * 只供其所属的EventLoop在IO线程调用,生命周期与EventLoop相等
         */
        class Poller:private noncopyable{
            public:
                using ChannelList = std::vector<Channel*>;
                Poller(EventLoop* loop);
                ~Poller();
                Timestamp poll(int timeoutMs,ChannelList* activeChannels);


                void updateChannel(Channel* channel);
                void assertInLoopThread(){
                    ownerloop_->assertInLoopThread();
                }
            private:
                void fillActiveChannels(int numEvents,ChannelList* activeChannels)const;
                using PollFdList = std::vector<struct pollfd>;
                using ChannelMap = std::map<int,Channel*>;

                EventLoop* ownerloop_;
                PollFdList pollfds_;
                ChannelMap channels_;
        };
    }
}

#endif //MUDUO_NET_INCLUDE_POLLER_H_
