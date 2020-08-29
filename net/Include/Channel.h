//
// Created by zrzzzlll on 2020/4/2.
//

#ifndef MUDUO_NET_INCLUDE_CHANNEL_H_
#define MUDUO_NET_INCLUDE_CHANNEL_H_

#include <functional>
#include "noncopyable.h"
namespace muduo{
    namespace net{
        class EventLoop;
        //每个Channel只属于某一个EventLoop(即Channel只属于某一个线程)
        //所有的函数都必须在IO线程中调用，所以不必加锁
        class Channel:private noncopyable{
            public:
                //回调函数类型
                typedef std::function<void ()>EventCallBack;
                Channel(EventLoop* loop,int fd);
                void handleEvent();
                /*
                 * 设置相应的回调函数
                 */
                void setReadCallBack(const EventCallBack& cb){ readCallBack_ = cb; }
                void setWriteCallBack(const EventCallBack& cb){ writeCallBack_ = cb; }
                void setErrorCallBack(const EventCallBack& cb){ errorCallBack_ = cb; }

                int fd()const{ return fd_; }
                int events()const{ return events_; }
                void set_revents(int revt){ revents_ = revt; }
                bool isNoneEvent()const{ return events_ == kNoneEvent; }

                void enableReading(){ events_ |= kReadEvent;update();}
                void enableWriting(){ events_ |= kWriteEvent;update();}
                void disableReading(){ events_ &= ~kReadEvent;update(); }
                void disableWriting(){ events_ &= ~kWriteEvent;update();}
                void disableAll(){ events_ = kNoneEvent;update(); }
                bool isReading(){ return events_ & kReadEvent; }
                bool isWriting(){ return events_ & kWriteEvent; }
                //被Poller类调用
                int index(){ return index_; }
                void set_index(int idx){ index_ = idx; };

                EventLoop* ownerLoop(){ return loop_; }
            private:
                void update();

                static const int kNoneEvent;
                static const int kReadEvent;
                static const int kWriteEvent;

                EventLoop* loop_;
                const int fd_;
                /*
                 * events是关心的事件
                 * revents是目前活跃的事件
                 */
                int events_;
                int revents_;
                int index_;//use by poller

                EventCallBack readCallBack_;
                EventCallBack writeCallBack_;
                EventCallBack errorCallBack_;
        };
    }
}
#endif //MUDUO_NET_INCLUDE_CHANNEL_H_
