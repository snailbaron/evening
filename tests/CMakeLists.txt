add_executable (evening_tests
    container.cpp
    subscriber.cpp
    subscription.cpp
)
target_link_libraries (evening_tests evening Catch2::Catch2WithMain)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    target_compile_options(evening_tests PRIVATE "/wd4996")
endif ()
