cmake_minimum_required(VERSION 3.19)
project(MandelbrotISPC)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)


set(ISPC_SOURCE ${CMAKE_SOURCE_DIR}/src/mandelbrot.ispc)
set(ISPC_OBJECT ${CMAKE_SOURCE_DIR}/lib/mandelbrot_ispc.o)
set(ISPC_HEADER ${CMAKE_SOURCE_DIR}/include/mandelbrot_ispc.h)
add_custom_command(
    OUTPUT ${ISPC_OBJECT} ${ISPC_HEADER}
    COMMAND ispc -g -O3 --target=avx2 --opt=disable-fma ${ISPC_SOURCE} -o ${ISPC_OBJECT} -h ${ISPC_HEADER} 
    DEPENDS ${ISPC_SOURCE}
    COMMENT "Compiling ISPC source to object and header files"
)
add_custom_target(mandelbrot_ispc ALL DEPENDS ${ISPC_OBJECT} ${ISPC_HEADER})


find_package(PNG REQUIRED)
include_directories(./include ../common ${PNG_INCLUDE_DIR})


add_executable(MandelbrotISPC main.cpp "./src/mandelbrotSerial.cpp" "../common/img.cpp" "./src/tasksys.cpp")
link_directories(${CMAKE_SOURCE_DIR}/lib)
target_link_libraries(MandelbrotISPC PRIVATE ${PNG_LIBRARY} ${ISPC_OBJECT} m pthread)