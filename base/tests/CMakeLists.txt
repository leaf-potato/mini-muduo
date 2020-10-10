include_directories(../../base)
add_executable(mutex_test Mutex_Test.cpp)
target_link_libraries(mutex_test muduo_base)

add_executable(atomic_test Atomic_Test.cpp)
target_link_libraries(atomic_test muduo_base)

add_executable(currentthread_test CurrentThread_Test.cpp)
target_link_libraries(currentthread_test muduo_base)

add_executable(thread_test Thread_Test.cpp)
target_link_libraries(thread_test muduo_base)

add_executable(timestamp_test Timestamp_Test.cpp)
target_link_libraries(timestamp_test muduo_base)