cmake_minimum_required(VERSION 3.15)
project(mini-muduo)

set(CMAKE_CXX_STANDARD 14)
set(BASE_INC_DIR ./base/Include)
set(BASE_SRC_DIR ./base/Src)

include_directories(${BASE_INC_DIR})
add_subdirectory(./net)
add_executable(mini-muduo main.cpp ${BASE_SRC_DIR})