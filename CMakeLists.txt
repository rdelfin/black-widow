cmake_minimum_required(VERSION 2.6)
project(black-widow)

set(BOOST_ROOT "C:/boost_1_59_0")
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost COMPONENTS REQUIRED system regex)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES src/main.cpp src/HttpResponse.cpp src/HttpRequest.cpp src/HttpGetRequest.cpp src/RegexParser.cpp)
add_executable(black-widow ${SOURCE_FILES})

include_directories(
        include
        ${Boost_INCLUDE_DIRS}
)

if(WIN32)
	add_definitions(-D_WIN32_WINDOWS)
	add_definitions(-D_WIN32_WINNT=0x0A00)
	target_link_libraries(black-widow wsock32 ws2_32 ${Boost_LIBRARIES})
else()
	find_package(Threads)
	target_link_libraries(black-widow ${CMAKE_THREAD_LIBS_INIT} ${Boost_LIBRARIES})
endif()

