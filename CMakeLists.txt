cmake_minimum_required(VERSION 3.15)
project(MiniOSSim VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/W4 /permissive-)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
    if(WIN32)
        add_link_options(-static)
    endif()
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

enable_testing()

# Sanity Test
add_executable(test_sanity tests/test_sanity.cpp)
add_test(NAME test_sanity COMMAND test_sanity)
