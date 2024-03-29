# vim: et ts=4 sts=4 sw=4 tw=0

# ==== Define cmake build policies that affect compilation and linkage default behaviors
#
# Set the JSONCPP_NEWEST_VALIDATED_POLICIES_VERSION string to the newest cmake version
# policies that provide successful builds. By setting JSONCPP_NEWEST_VALIDATED_POLICIES_VERSION
# to a value greater than the oldest policies, all policies between
# JSONCPP_OLDEST_VALIDATED_POLICIES_VERSION and CMAKE_VERSION (used for this build)
# are set to their NEW behaivor, thereby suppressing policy warnings related to policies
# between the JSONCPP_OLDEST_VALIDATED_POLICIES_VERSION and CMAKE_VERSION.
#
# CMake versions greater than the JSONCPP_NEWEST_VALIDATED_POLICIES_VERSION policies will
# continue to generate policy warnings "CMake Warning (dev)...Policy CMP0XXX is not set:"
#
set(JSONCPP_OLDEST_VALIDATED_POLICIES_VERSION "3.1.0")
set(JSONCPP_NEWEST_VALIDATED_POLICIES_VERSION "3.13.2")
cmake_minimum_required(VERSION ${JSONCPP_OLDEST_VALIDATED_POLICIES_VERSION})
if("${CMAKE_VERSION}" VERSION_LESS "${JSONCPP_OLDEST_VALIDATED_POLICIES_VERSION}")
    #Set and use the newest available cmake policies that are validated to work
    set(JSONCPP_CMAKE_POLICY_VERSION "${CMAKE_VERSION}")
else()
    set(JSONCPP_CMAKE_POLICY_VERSION "${JSONCPP_OLDEST_VALIDATED_POLICIES_VERSION}")
endif()
cmake_policy(VERSION ${JSONCPP_CMAKE_POLICY_VERSION})
#
# Now enumerate specific policies newer than JSONCPP_NEWEST_VALIDATED_POLICIES_VERSION
# that may need to be individually set to NEW/OLD
#
foreach(pnew "") # Currently Empty
    if(POLICY ${pnew})
        cmake_policy(SET ${pnew} NEW)
    endif()
endforeach()
foreach(pold "") # Currently Empty
    if(POLICY ${pold})
        cmake_policy(SET ${pold} OLD)
    endif()
endforeach()

# ==== Define language standard configurations requiring at least c++11 standard
if(CMAKE_CXX_STANDARD EQUAL "98" )
    message(FATAL_ERROR "CMAKE_CXX_STANDARD:STRING=98 is not supported.")
endif()

#####
##  Set the default target properties
if(NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 11) # Supported values are ``11``, ``14``, and ``17``.
endif()
if(NOT CMAKE_CXX_STANDARD_REQUIRED)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()
if(NOT CMAKE_CXX_EXTENSIONS)
    set(CMAKE_CXX_EXTENSIONS OFF)
endif()

# ====

# Ensures that CMAKE_BUILD_TYPE has a default value
if(NOT DEFINED CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING
        "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel Coverage.")
endif()

project(JSONCPP
        VERSION 1.9.0 # <major>[.<minor>[.<patch>[.<tweak>]]]
        LANGUAGES CXX)

set( JSONCPP_VERSION ${JSONCPP_VERSION_MAJOR}.${JSONCPP_VERSION_MINOR}.${JSONCPP_VERSION_PATCH} )
message(STATUS "JsonCpp Version: ${JSONCPP_VERSION_MAJOR}.${JSONCPP_VERSION_MINOR}.${JSONCPP_VERSION_PATCH}")
set( JSONCPP_SOVERSION 21 )

if (ARM64)
option(JSONCPP_WITH_TESTS "Compile and (for jsoncpp_check) run JsonCpp test executables" OFF)
else (ARM64)
option(JSONCPP_WITH_TESTS "Compile and (for jsoncpp_check) run JsonCpp test executables" OFF)
endif (ARM64)
option(JSONCPP_WITH_POST_BUILD_UNITTEST "Automatically run unit-tests as a post build step" OFF)
option(JSONCPP_WITH_WARNING_AS_ERROR "Force compilation to fail if a warning occurs" OFF)
option(JSONCPP_WITH_STRICT_ISO "Issue all the warnings demanded by strict ISO C and ISO C++" ON)
option(JSONCPP_WITH_PKGCONFIG_SUPPORT "Generate and install .pc files" ON)
option(JSONCPP_WITH_CMAKE_PACKAGE "Generate and install cmake package files" ON)
if ( ANDROID )
option(BUILD_SHARED_LIBS "Build jsoncpp_lib as a shared library." OFF)
else ()
option(BUILD_SHARED_LIBS "Build jsoncpp_lib as a shared library." ON)
endif ( ANDROID )

if (ARM64_LOCAL_CROSS)
set (TOOLS ${ARM_TOOLCHAIN})
set (CMAKE_FIND_ROOT_PATH ${TOOLS}/aarch64-linux-gnu/lib)
set (PREFIX aarch64-linux-gnu-)
set (CMAKE_C_COMPILER ${TOOLS}/bin/${PREFIX}gcc)
set (CMAKE_CXX_COMPILER ${TOOLS}/bin/${PREFIX}g++)
set (CMAKE_C_LINKER_PREFERENCE ${TOOLS}/bin/${PREFIX}ld)
set (CMAKE_CXX_LINKER_PREFERENCE ${TOOLS}/bin/${PREFIX}ld)
endif()
# Enable runtime search path support for dynamic libraries on OSX
if(APPLE)
    set(CMAKE_MACOSX_RPATH 1)
endif()

# Adhere to GNU filesystem layout conventions
include(GNUInstallDirs)

set(DEBUG_LIBNAME_SUFFIX "" CACHE STRING "Optional suffix to append to the library name for a debug build")

# Set variable named ${VAR_NAME} to value ${VALUE}
function(set_using_dynamic_name VAR_NAME VALUE)
    set( "${VAR_NAME}" "${VALUE}" PARENT_SCOPE)
endfunction()

set( JSONCPP_USE_SECURE_MEMORY "0" CACHE STRING "-D...=1 to use memory-wiping allocator for STL" )

# File version.h is only regenerated on CMake configure step
configure_file( "${PROJECT_SOURCE_DIR}/src/lib_json/version.h.in"
                "${PROJECT_BINARY_DIR}/include/json/version.h"
                NEWLINE_STYLE UNIX )
configure_file( "${PROJECT_SOURCE_DIR}/version.in"
                "${PROJECT_BINARY_DIR}/version"
                NEWLINE_STYLE UNIX )

macro(UseCompilationWarningAsError)
    if(MSVC)
        # Only enabled in debug because some old versions of VS STL generate
        # warnings when compiled in release configuration.
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /WX ")
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
        if(JSONCPP_WITH_STRICT_ISO)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic-errors")
        endif()
    endif()
endmacro()

# Include our configuration header
include_directories( ${jsoncpp_SOURCE_DIR}/include )

if(MSVC)
    # Only enabled in debug because some old versions of VS STL generate
    # unreachable code warning when compiled in release configuration.
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /W4 ")
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    # using regular Clang or AppleClang
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wconversion -Wshadow -Werror=conversion -Werror=sign-compare")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    # using GCC
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wconversion -Wshadow -Wextra")
    # not yet ready for -Wsign-conversion

    if(JSONCPP_WITH_STRICT_ISO)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic")
    endif()
    if(JSONCPP_WITH_WARNING_AS_ERROR)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror=conversion")
    endif()
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
    # using Intel compiler
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wconversion -Wshadow -Wextra -Werror=conversion")

    if(JSONCPP_WITH_STRICT_ISO AND NOT JSONCPP_WITH_WARNING_AS_ERROR)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic")
    endif()
endif()

find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif(CCACHE_FOUND)

if(JSONCPP_WITH_WARNING_AS_ERROR)
    UseCompilationWarningAsError()
endif()

if(JSONCPP_WITH_PKGCONFIG_SUPPORT)
    configure_file(
        "pkg-config/jsoncpp.pc.in"
        "pkg-config/jsoncpp.pc"
        @ONLY)
    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/pkg-config/jsoncpp.pc"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig")
endif()

if(JSONCPP_WITH_CMAKE_PACKAGE)
        install(EXPORT jsoncpp
                DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/jsoncpp
                FILE        jsoncppConfig.cmake)
endif()


# Build the different applications
add_subdirectory( src )

#install the includes
add_subdirectory( include )

