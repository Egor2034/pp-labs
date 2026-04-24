cmake_minimum_required(VERSION 3.12)

project("lab4" LANGUAGES CXX CUDA)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable("lab4" "main.cu")

target_link_libraries("lab4" ${CUDA_LIBRARIES})

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
    endif()
    add_compile_options("/utf-8")
endif()