cmake_minimum_required(VERSION 3.12)
project("lab3" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable("lab3" "main.cpp" "matrix.hpp")


target_include_directories("lab3" PRIVATE "C:/Program Files (x86)/Microsoft SDKs/MPI/Include")
target_link_directories("lab3" PRIVATE "C:/Program Files (x86)/Microsoft SDKs/MPI/Lib/x64")
target_link_libraries("lab3"PRIVATE msmpi)
    
target_link_options("lab3" PRIVATE "/STACK:134217728")


set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -DDEBUG")