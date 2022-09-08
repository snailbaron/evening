cmake_minimum_required (VERSION 3.8)

project (evening)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
        "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    add_compile_options (-Wall -Wextra -Werror)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    add_compile_options (/W4 /WX /EHsc)
endif ()

add_library (evening INTERFACE)
target_sources (evening INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include/evening.hpp)
target_include_directories (evening INTERFACE include/)

install (TARGETS evening EXPORT snailbaron::evening)
export (TARGETS evening FILE ${CMAKE_BINARY_DIR}/evening-config.cmake)

if(NOT TARGET Catch2::Catch2WithMain)
    add_subdirectory(deps/Catch2)
endif()

add_subdirectory (tests)
enable_testing ()
add_test (NAME evening_tests COMMAND evening_tests)
