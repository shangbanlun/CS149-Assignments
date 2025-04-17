cmake_minimum_required(VERSION 3.10)
project(MandelbrotThreads)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Find the PNG library
find_package(PNG REQUIRED)
include_directories(../common ${PNG_INCLUDE_DIR})

# Add the executable
add_executable(mandelbrot
    main.cpp
    ../common/img.cpp
    mandelbrotSerial.cpp
    mandelbrotThread.cpp
)

target_link_libraries(mandelbrot PRIVATE ${PNG_LIBRARY} pthread m)