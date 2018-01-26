add_executable (evening_tests
    container.cpp
    subscriber.cpp
    subscription.cpp
    main.cpp)
target_link_libraries (evening_tests evening)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    target_compile_options(evening_tests PRIVATE "/wd4996")
endif ()
