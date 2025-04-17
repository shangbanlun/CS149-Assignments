cmake_minimum_required(VERSION 3.10)
project(MandelbrotThreads)
set(CMAKE_BUILD_TYPE Debug)


set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(include)
# Add the executable
add_executable(myexp
    "main.cpp"
    "src/logger.cpp"
    "src/CS149intrin.cpp"
)