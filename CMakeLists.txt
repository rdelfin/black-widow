cmake_minimum_required(VERSION 3.3)
project(Crawler)

MESSAGE(Hello)

set(BOOST_ROOT "C:/boost_1_59_0")
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost COMPONENTS system regex REQUIRED)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(${Boost_INCLUDE_DIRS})

set(SOURCE_FILES main.cpp)
add_executable(Crawler ${SOURCE_FILES})

target_link_libraries(Crawler wsock32 ws2_32 ${Boost_LIBRARIES})

# target_link_libraries(Crawler ${})