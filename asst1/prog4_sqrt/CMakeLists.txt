cmake_minimum_required(VERSION 3.19)
project(sqrt)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
add_compile_options(-mavx2)

set(ISPC_SOURCE ${CMAKE_SOURCE_DIR}/src/sqrt.ispc)
set(ISPC_OBJECT ${CMAKE_SOURCE_DIR}/lib/sqrt_ispc.o)
set(ISPC_HEADER ${CMAKE_SOURCE_DIR}/include/sqrt_ispc.h)
add_custom_command(
    OUTPUT ${ISPC_OBJECT} ${ISPC_HEADER}
    COMMAND ispc -O3 --target=avx2 --opt=disable-fma ${ISPC_SOURCE} -o ${ISPC_OBJECT} -h ${ISPC_HEADER} 
    DEPENDS ${ISPC_SOURCE}
    COMMENT "Compiling ISPC source to object and header files"
)
add_custom_target(sqrt_ispc ALL DEPENDS ${ISPC_OBJECT} ${ISPC_HEADER})


include_directories(./include ../common)


add_executable(sqrt main.cpp "./src/sqrtSerial.cpp" "./src/sqrtAXV2.cpp" "./src/tasksys.cpp")
link_directories(${CMAKE_SOURCE_DIR}/lib)
target_link_libraries(sqrt PRIVATE ${ISPC_OBJECT} m pthread)