cmake_minimum_required(VERSION 3.12)
project("lab2" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable("lab2" "main.cpp")

find_package(OpenMP REQUIRED)
target_link_libraries("lab2" PRIVATE OpenMP::OpenMP_CXX)
target_compile_options("lab2" PRIVATE ${OpenMP_CXX_FLAGS})

if(MSVC)
    target_link_options("lab2" PRIVATE "/STACK:67108864") 
else()
    target_link_options("lab2" PRIVATE "-Wl,--stack,67108864")  
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options("lab2" PRIVATE /W4)
else()
    target_compile_options("lab2" PRIVATE -Wall -Wextra -Wpedantic)
endif()

set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -DDEBUG")