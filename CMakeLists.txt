cmake_minimum_required(VERSION 3.0)
project(snake CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../bin")
find_package(Curses REQUIRED)

include_directories(
    ${CURSES_INCLUDE_DIR}
)

set(source main.cpp)

##------------- COMPILE FLAGS
message("CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
set(CMAKE_CXX_FLAGS            "${CMAKE_CXX_FLAGS}         -pthread -std=c++11 -Wall -march=native ")
set(CMAKE_C_FLAGS              "${CMAKE_C_FLAGS}           ")
set(CMAKE_CXX_FLAGS_DEBUG      "${CMAKE_CXX_FLAGS_DEBUG}   -O0 -g")
set(CMAKE_CXX_FLAGS_RELEASE    "${CMAKE_CXX_FLAGS_RELEASE} -O3")

add_executable(snake ${source})

target_link_libraries(
    snake
    ${CURSES_LIBRARIES}
)
