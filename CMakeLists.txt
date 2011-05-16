project(Logger C CXX)

cmake_minimum_required(VERSION 2.8)

enable_testing()

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# enable all warnings
if(CMAKE_COMPILER_IS_GNUCXX)
    add_definitions("-Wall -Wextra")
endif(CMAKE_COMPILER_IS_GNUCXX)

# if not build type is specified, default to RelWithDebInfo
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING
            "Choose the type of build desired. Valid options are: None Debug Release RelWithDebInfo MinSizeRel."
            FORCE)
endif(NOT CMAKE_BUILD_TYPE)

# disable qDebugs when we compile in Release
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_definitions(-DQT_NO_DEBUG_OUTPUT)
endif(CMAKE_BUILD_TYPE STREQUAL "Release")

add_subdirectory(src)
add_subdirectory(tests)
