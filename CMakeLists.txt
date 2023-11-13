cmake_minimum_required(VERSION 3.20)

project(mylib CXX)

option(MYLIB_ENABLE_GTEST "MylibでのGoogleTestの有効化" ON)
option(MYLIB_ENABLE_TEST "MylibでのTestの有効化" ON)
option(CMAKE_OPTIMIZE_DEPENDENCIES "Optimize Dependencies" ON)

# Setting Build Type
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
endif()

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -march=native") # -flto
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}") # -fsanitize=address
set(CMAKE_CXX_EXTENSIONS OFF) # disable gnu extension

message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_LIST_DIR})
  # compile_commands.jsonを出力する
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
  message(STATUS "Google Test: ${MYLIB_ENABLE_GTEST}")
  message(STATUS "Test: ${MYLIB_ENABLE_TEST}")
else()
  set(MYLIB_ENABLE_GTEST OFF)
  set(MYLIB_ENABLE_TEST OFF)
endif()

# Errors
set(CMAKE_CXX_FLAGS
    "-Wall -Wextra -Werror -Wnon-virtual-dtor -Wfloat-equal -Winline")
# For pybind11
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -fvisibility=hidden -ffast-math")

find_package(GTest)

file(GLOB MYLIB_SOURCES ${CMAKE_CURRENT_LIST_DIR}/src/**/*.cpp
     ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)

add_library(mylib STATIC ${MYLIB_SOURCES})

set_property(TARGET mylib PROPERTY CXX_STANDARD 20)

target_include_directories(mylib PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)

if(${MYLIB_ENABLE_TEST})
  find_package(benchmark CONFIG REQUIRED)
  file(GLOB TEST_SOURCES ${CMAKE_CURRENT_LIST_DIR}/tests/*.cpp
       ${CMAKE_CURRENT_LIST_DIR}/tests/**/*.cpp)
  foreach(s ${TEST_SOURCES})
    get_filename_component(t ${s} NAME_WE)
    add_executable(${t} ${s})
    set_property(TARGET ${t} PROPERTY CXX_STANDARD 20)
    target_link_libraries(${t} PUBLIC mylib benchmark::benchmark)
    target_include_directories(
      ${t} SYSTEM PUBLIC ${CMAKE_CURRENT_LIST_DIR}/external/autodiff)
  endforeach()
endif()

# if(${MYLIB_ENABLE_GTEST})
#   file(GLOB TEST_SOURCES ${CMAKE_CURRENT_LIST_DIR}/gtests/*.cpp
#        ${CMAKE_CURRENT_LIST_DIR}/gtests/**/*.cpp)
#   add_executable(mylib-gtest ${TEST_SOURCES})
#   set_property(TARGET mylib-gtest PROPERTY CXX_STANDARD 20)
#   target_link_libraries(mylib-gtest PUBLIC mylib GTest::gtest_main)
# endif()
