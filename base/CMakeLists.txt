set(Base_Src
        CurrentThread.cpp
        Thread.cpp
        CountDownLatch.cpp
        Timestamp.cpp
        Date.cpp)

add_library(muduo_base ${Base_Src})
target_link_libraries(muduo_base pthread rt)

add_subdirectory(tests)
