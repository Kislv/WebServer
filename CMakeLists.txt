cmake_minimum_required(VERSION 3.10)

ADD_DEFINITIONS ( -levent_core )

# set the project name
project(WebServer)

# add the executable
add_executable(WebServer src/main.cpp)
