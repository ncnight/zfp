cmake_minimum_required(VERSION 3.16)
project(zfp_extraction)

set(CMAKE_CXX_STANDARD 14)

set(ZFP_DIR /home/nithin/Workspace/condensa/condensa/experimental/zfp)
set(ZFP_LIB ${ZFP_DIR}/build/lib/libzfp.so)


if(NOT DEFINED ZFP_DIR)
    message( FATAL_ERROR "ZFP_DIR is not set!" )
endif(NOT DEFINED ZFP_DIR)

if(NOT DEFINED ZFP_LIB)
    message( FATAL_ERROR "ZFP_LIB is not set!" )
endif(NOT DEFINED ZFP_LIB)

include_directories(
        ${ZFP_DIR}/array
        ${ZFP_DIR}/array/zfp
        ${ZFP_DIR}/include
)


add_executable(zfp_extraction main.cpp)
target_link_libraries(zfp_extraction ${ZFP_LIB})

