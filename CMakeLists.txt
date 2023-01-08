cmake_minimum_required(VERSION 3.24)
project(bank_system)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)
link_libraries(libeasyx.a)

add_executable(bank_system init.c functions.h menu.c function.c GUI.cpp)