//
// Created by zrzzzlll on 2020/8/30.
//

#ifndef MINI_MUDUO_NET_INCLUDE_TCPSERVER_H_
#define MINI_MUDUO_NET_INCLUDE_TCPSERVER_H_


namespace muduo{
    namespace net{
        class Acceptor;
        class EventLoop;
        class TcpServer: private noncopyable{
            public:
                TcpServer(EventLoop* loop,const InetAddress& listenAddr);
                ~TcpServer();
                void start();
                void setConnectionCallback(const ConnectionCallback& cb){
                    connectionCallback_ = cb;
                }
                void setMessageCallback(const MessageCallback& cb){
                    messageCallback_ = cb;
                }
            private:
                //不是线程安全的，但是在in loop中调用
                void newConnection(int sockfd,const InetAddress& peerAddr);
                using ConnectionMap = std::map<std::string,TcpConnectionPtr>;
                EventLoop* loop_;
                const std::string name;
                bool started_;
                int nextConnId_;
                std::shared_ptr<Acceptor>accepter_;
                ConnectionCallback connectionCallback_;
                MessageCallback messageCallback_;
                /*
                 * TcpServer中所有的连接
                 */
                ConnectionMap connections_;
        };
    }
}

#endif //MINI_MUDUO_NET_INCLUDE_TCPSERVER_H_
