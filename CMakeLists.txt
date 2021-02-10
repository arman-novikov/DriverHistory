cmake_minimum_required(VERSION 3.5)

set(TESTS ON)
set(PROJECT_NAME driver_history)
project(${PROJECT_NAME} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(THREADS_PREFER_PTHREAD_FLAG ON)

include_directories(tests)

add_library(DriverHistory STATIC DriverHistory.cpp DriverHistory.hpp)

if(TESTS)
    find_package(Threads REQUIRED)
    include_directories("tests/")
    add_executable(${PROJECT_NAME} tests/main.cpp)
    target_link_libraries(${PROJECT_NAME}
        PRIVATE DriverHistory gtest Threads::Threads
    )
else()
    add_executable(${PROJECT_NAME} main.cpp)
    target_link_libraries(${PROJECT_NAME}
        PRIVATE DriverHistory
    )
endif()
