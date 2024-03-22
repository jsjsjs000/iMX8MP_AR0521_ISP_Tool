cmake_minimum_required(VERSION 3.16.0)

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)
set(CMAKE_SYSROOT "${OECORE_NATIVE_SYSROOT}")
set(CMAKE_PREFIX_PATH ${CMAKE_SYSROOT}/usr/lib/cmake)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

project(AR0521_iMX8MP_ISP_Tool)

add_definitions(-Wall)
add_definitions(-fPIC)
add_definitions(-DPRINT_LOG)
add_definitions(-DISP_VIDEO_TEST)

include_directories(
	lib/json_helper
	lib/vvcam/common
	lib/cam_device/include
	lib/cam_device/vvcam/common
	lib/jsoncpp/include
	lib/utils3rd/include
	lib/mediacontrol/include_api
	lib/cam_device/utils/base64
)
file(GLOB pub_headers *.h)

add_executable(iMX8MP_AR0521_ISP_Tool
	main.cpp
)

add_subdirectory(${PROJECT_SOURCE_DIR}/lib/jsoncpp jsoncpp)

target_link_libraries(iMX8MP_AR0521_ISP_Tool
	${LIB_PTHREAD_NAME}
	jsoncpp_lib
	${LIB_LOG_NAME}
)


# 	Cmake cross compile
# https://cmake.org/cmake/help/book/mastering-cmake/chapter/Cross%20Compiling%20With%20CMake.html
# https://cmake.org/cmake/help/latest/manual/cmake-toolchains.7.html
# https://kubasejdak.com/how-to-cross-compile-for-embedded-with-cmake-like-a-champ

# https://community.nxp.com/t5/i-MX-Processors/How-to-Build-Cross-Compile-Toolchains-For-i-MX8M/td-p/782981
# https://kubasejdak.com/how-to-cross-compile-for-embedded-with-cmake-like-a-champ
# https://github.com/bstubert/embeddeduse/blob/master/BlogPosts/CMakeToolchainFile/toolchain-imx6.cmake
# https://community.nxp.com/t5/i-MX-Processors-Knowledge-Base/i-MX-8-Hello-World-Linux-ArmV7a-32-bit-and-ArmV8a-64-bit/ta-p/1100128
# https://thesofproject.github.io/latest/getting_started/nxp/sof_imx_user_guide.html
