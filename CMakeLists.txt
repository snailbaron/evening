cmake_minimum_required (VERSION 3.1)

project (evening)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
        "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set (flags "-std=c++17 -Wall -Wextra -Werror")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set (flags "/std:c++17 /W4 /WX /EHsc")
endif ()

if (NOT CONFIGURED_ONCE)
    set (CMAKE_CXX_FLAGS "${flags}"
        CACHE STRING "Compilation flags for all build types." FORCE)
endif()

add_library (evening INTERFACE)
target_sources (evening INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/evening/include/evening.hpp)
target_include_directories (evening INTERFACE evening/include/)

add_subdirectory (tests)
enable_testing ()
add_test (NAME evening_tests COMMAND evening_tests)
