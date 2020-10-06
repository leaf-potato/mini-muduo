include_directories(../../base)
add_executable(mutex_test Mutex_Test.cpp)
target_link_libraries(mutex_test muduo_base)

add_executable(atomic_test Atomic_Test.cpp)
target_link_libraries(atomic_test muduo_base)