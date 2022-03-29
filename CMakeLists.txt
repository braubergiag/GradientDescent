cmake_minimum_required(VERSION 3.19)
project(GradientDescent)

set(CMAKE_CXX_STANDARD 17)
add_subdirectory(include)
add_executable(GradientDescent main.cpp GradientDescent.cpp ../include)