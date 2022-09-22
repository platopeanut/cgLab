cmake_minimum_required(VERSION 3.23)
project(cgLab)

set(CMAKE_CXX_STANDARD 11)

file(GLOB display display/**)

add_executable(template main.cpp ${display})
