cmake_minimum_required (VERSION 3.16)

project(AdventOfCodeParent LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
    "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(warnings "-Wall -Wextra -Werror")
    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        add_definitions(-DGNU)
    else()
        add_definitions(-DCLANG)
    endif()
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(warnings "/W4 /WX /EHsc")
    add_definitions(-DMSVC)
    set(CMAKE_CXX_STANDARD 23)
    add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/std:c++latest>)
endif()
if (NOT CONFIGURED_ONCE)
    set(CMAKE_CXX_FLAGS "${warnings}"
        CACHE STRING "Flags used by the compiler during all build types." FORCE)
    set(CMAKE_C_FLAGS   "${warnings}"
        CACHE STRING "Flags used by the compiler during all build types." FORCE)
endif()

if (${CMAKE_HOST_WIN32})
    add_definitions(-DWIN32)
elseif(${CMAKE_HOST_LINUX})
    add_definitions(-D__linux__)
endif()

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    add_definitions(-DDEBUG)
else()
    add_definitions(-DRELEASE)
endif()

#set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD ON)
#set(CMAKE_CXX_COMPILER_IMPORT_STD ON)


include(FetchContent)
#################################
# dstbstr/CppCore
#################################

#get_directory_property(LocalCore CppCore )
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/CppCore)
    add_subdirectory("CppCore")
else()
    FetchContent_Declare(CppCore GIT_REPOSITORY https://github.com/dstbstr/CppCore GIT_TAG origin/main)
    FetchContent_MakeAvailable(CppCore)
endif()
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/CliParser)
    add_subdirectory("CliParser")
else()
    FetchContent_Declare(CliParser GIT_REPOSITORY https://github.com/dstbstr/CliParser GIT_TAG origin/main)
    FetchContent_MakeAvailable(CliParser)
endif()

# Compile Time Regular Expressions
FetchContent_Declare(ctre GIT_REPOSITORY https://github.com/hanickadot/compile-time-regular-expressions GIT_TAG origin/main)
FetchContent_MakeAvailable(ctre)

# Include sub-projects.
add_subdirectory ("AdventOfCode")
