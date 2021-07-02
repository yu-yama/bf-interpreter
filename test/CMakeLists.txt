cmake_policy(SET CMP0050 NEW)

cmake_minimum_required(VERSION 3.17)

project(BFInterpreter-Test)

message("compiler: ${CMAKE_CXX_COMPILER}")
if(NOT "${CMAKE_CXX_STANDARD}")
    set(CMAKE_CXX_STANDARD 17)
endif()
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)
message("std=c++${CMAKE_CXX_STANDARD}")

set(SRC_DIR ${PROJECT_SOURCE_DIR}/)
set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/../include)

if(MSVC)
    message("MSVC: Affirmative")
    add_compile_options(/W4 /permissive- /sdl)
else()
    message("MSVC: Negative")
    add_compile_options(-Wall -Werror -Wextra -Wconversion -Wno-sign-conversion -Wshadow)
endif()

enable_testing()

add_subdirectory(${PROJECT_SOURCE_DIR}/googletest)
include(GoogleTest)

list(APPEND Tests InstructionsTest SampleTest)
list(APPEND Files instructions_test.cpp sample_test.cpp)

foreach(Test File IN ZIP_LISTS Tests Files)
    add_executable(${Test} ${SRC_DIR}/${File})
    target_link_libraries(${Test} gtest_main)
    target_include_directories(${Test} PUBLIC ${INCLUDE_DIR})
    gtest_discover_tests(${Test})
endforeach()
