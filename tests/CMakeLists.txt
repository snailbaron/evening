add_executable (evening_test
    container.cpp
    subscriber.cpp
    subscription.cpp
    main.cpp)
target_link_libraries (evening_test evening)
set_property (TARGET evening_test PROPERTY CXX_STANDARD 17)

add_test (evening_test evening_test)
