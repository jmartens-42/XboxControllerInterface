cmake_minimum_required(VERSION 3.10)
SET(PROJ_NAME JoystickTest)
PROJECT(${PROJ_NAME})



set(CMAKE_CXX_COMPILER /usr/bin/g++-10)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-O3 ")
set(CMAKE_VERBOSE_MAKEFILE on)

find_package(Threads REQUIRED)

include_directories(include/)


SET(SRC_FILES src/controller_handler.cpp
              src/utilities.cpp
              )


ADD_EXECUTABLE(JoystickTest joystick_main.cpp ${SRC_FILES})

ADD_LIBRARY(XboxControllerLib ${SRC_FILES})

target_link_libraries(JoystickTest Threads::Threads)
target_link_libraries(XboxControllerLib Threads::Threads)
