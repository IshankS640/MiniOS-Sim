cmake_minimum_required(VERSION 3.16)
project(minios_sim CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()
add_compile_options(-Wall -Wextra -Wpedantic)

file(GLOB_RECURSE CORE_SRC CONFIGURE_DEPENDS src/*.cpp)
list(REMOVE_ITEM CORE_SRC ${CMAKE_SOURCE_DIR}/src/main.cpp)

add_library(minios_core ${CORE_SRC})
target_include_directories(minios_core PUBLIC include)

add_executable(osim src/main.cpp)
target_link_libraries(osim minios_core)

enable_testing()
file(GLOB TEST_SRC CONFIGURE_DEPENDS tests/*.cpp)
foreach(t ${TEST_SRC})
  get_filename_component(name ${t} NAME_WE)
  add_executable(${name} ${t})
  target_link_libraries(${name} minios_core)
  add_test(NAME ${name} COMMAND ${name})
endforeach()
