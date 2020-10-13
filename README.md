## mini-muduo

#### 项目背景

该项目是本人在阅读《Linux服务端编程：使用muduo C++网络库》书籍同时学习`muduo`网络库过程中，为了加深对其架构设计的理解，了解具体实现细节和锻炼个人动手能力，参考其源码实现的`mini-muduo`网络库。

#### 实现顺序

**base**

- `copyable class` 和`nocoyable class`【可复制和不可复制类】 
- `MutexLock class` 和 `MutexLockGuard class`【互斥锁和互斥锁管理类】
- `Condition class`【条件变量类】
- `CountDownLatch class`【倒计时类】
- `BlockingQueue class`【阻塞队列类】
- `AtomicIntegerT template class`【整数原子模板】
- `Exception class`【异常类】
- `Thread class`【线程类】
- `ThreadPool class`【线程池类】
- `Timestamp class`【时间戳类】
- `Date class`【日期类】
- `ThreadLocal class`【线程存储类】

**net**

