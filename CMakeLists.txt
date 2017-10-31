cmake_minimum_required(VERSION 3.5)
project(xwg)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "-O3 -std=c++11 -Wall -Wctor-dtor-privacy -Wuninitialized -Winit-self -Wlogical-op -Wredundant-decls -Wshadow -Werror -Wno-unused -Wno-sign-compare -Wno-reorder -Wno-unused-variable")

add_library(a1-obj OBJECT a1.cpp)
add_library(main-obj OBJECT xor-new.cpp)
add_executable(xwg $<TARGET_OBJECTS:a1-obj> $<TARGET_OBJECTS:main-obj>)
