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
if(MSVC)
    message("Using MSVC compiler")
    set(CMAKE_CXX_FLAGS            "${CMAKE_CXX_FLAGS}         /bigobj /std:c++14 /arch:AVX /W3 /Zc:inline")
    set(CMAKE_CXX_FLAGS_DEBUG      "${CMAKE_CXX_FLAGS_DEBUG}   /MTd /Zc:forScope /Od /RTC1 /GS /Gm")
    set(CMAKE_CXX_FLAGS_RELEASE    "${CMAKE_CXX_FLAGS_RELEASE} /Ot /O2 /fp:precise /Gm-")
    add_definitions( -D_CRT_SECURE_NO_WARNINGS)
else(MSVC)
    message("Using GNU compiler")
    set(CMAKE_CXX_FLAGS            "${CMAKE_CXX_FLAGS}         -pthread -std=c++11 -Wall -march=native ")
    set(CMAKE_C_FLAGS              "${CMAKE_C_FLAGS}           ")
    set(CMAKE_CXX_FLAGS_DEBUG      "${CMAKE_CXX_FLAGS_DEBUG}   -O0 -g")
    set(CMAKE_CXX_FLAGS_RELEASE    "${CMAKE_CXX_FLAGS_RELEASE} -O3")
endif(MSVC)

add_executable(snake ${source})

target_link_libraries(
    snake
    ${CURSES_LIBRARIES}
)