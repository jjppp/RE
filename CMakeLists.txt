# cmake 最低版本需求
cmake_minimum_required(VERSION 3.13)

# 工程名称
project(RE)

# 设置
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -ggdb")

# include files
include_directories(include/)

# 编译源码生成目标
add_executable(RE src/Expr.cpp src/DFA.cpp src/NFA.cpp src/repl.cpp src/main.cpp src/utils.cpp)
