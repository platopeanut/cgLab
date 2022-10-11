cmake_minimum_required(VERSION 3.23)
project(cgLab)

set(CMAKE_CXX_STANDARD 11)

file(GLOB display display/**)
file(GLOB draw draw/**)
file(GLOB clipping clipping/**)
file(GLOB utils utils/**)

add_executable(template lifeCycle.cpp render.cpp ${display} ${draw} ${utils} ${clipping})
