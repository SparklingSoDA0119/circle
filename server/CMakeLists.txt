# Circle Server CMakeLists.txt

CMAKE_MINIMUM_REQUIRED(VERSION 3.11)

PROJECT(circle_srv)

ADD_COMPILE_OPTIONS(-std=c++17 -O3 -Wall -fno-aggressive-loop-optimizations -ffunction-sections -fdata-sections -fpermissive -Wno-error=implicit-function-declaration -g)

SET(CMAKE_VERBOSE_MAKEFILE true)

INCLUDE_DIRECTORIES("./")

FILE(GLOB CCSRV "./CircleServer.cpp")
FILE(GLOB CCSYSTEM "./system/*.cpp")

ADD_EXECUTABLE(${CMAKE_PROJECT_NAME} ${CCSRV} ${CCSYSTEM})
TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME} PocoFoundation)