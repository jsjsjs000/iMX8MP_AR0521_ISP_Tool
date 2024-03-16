cmake_minimum_required(VERSION 3.16.0)

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)
# set(TOOLCHAIN_PREFIX "/opt/ampliphy-vendor-xwayland/BSP-Yocto-NXP-i.MX8MP-PD23.1.0/sysroots/x86_64-phytecsdk-linux/usr/bin/aarch64-phytec-linux/aarch64-phytec-linux-")
# set(CMAKE_C_COMPILER "${TOOLCHAIN_PREFIX}gcc")
# set(CMAKE_CXX_COMPILER "${TOOLCHAIN_PREFIX}g++")
set(CMAKE_SYSROOT "/opt/ampliphy-vendor-xwayland/BSP-Yocto-NXP-i.MX8MP-PD23.1.0/sysroots/x86_64-phytecsdk-linux")
set(CMAKE_PREFIX_PATH ${CMAKE_SYSROOT}/usr/lib/cmake)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} ")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
# set(CMAKE_C_LDFLAGS "/opt/ampliphy-vendor-xwayland/BSP-Yocto-NXP-i.MX8MP-PD23.1.0/sysroots/x86_64-phytecsdk-linux")
# set(CMAKE_CXX_LDFLAGS "/opt/ampliphy-vendor-xwayland/BSP-Yocto-NXP-i.MX8MP-PD23.1.0/sysroots/x86_64-phytecsdk-linux")
# set(COMPILER_FLAGS " -march=armv7-a -marm -mfpu=neon  -mfloat-abi=hard -mcpu=cortex-a9")

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

add_executable(AR0521_iMX8MP_ISP_Tool
	main.cpp
)

add_subdirectory(${PROJECT_SOURCE_DIR}/lib/jsoncpp jsoncpp)

target_link_libraries(AR0521_iMX8MP_ISP_Tool
	${LIB_PTHREAD_NAME}
	jsoncpp_lib
	${LIB_LOG_NAME}
)
