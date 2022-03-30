cmake_minimum_required(VERSION 3.15)
project(GradientDescent)

set(CMAKE_CXX_STANDARD 17)
add_executable(GradientDescent
        ${CMAKE_SOURCE_DIR}/src/main.cpp
        ${CMAKE_SOURCE_DIR}/src/gradientdescent.cpp
        ${CMAKE_SOURCE_DIR}/src/ConsoleWrapper.cpp
        ${CMAKE_SOURCE_DIR}/src/functionhandler.cpp
        ${CMAKE_SOURCE_DIR}/src/model.cpp
        )

target_include_directories(GradientDescent PRIVATE
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/include
        )