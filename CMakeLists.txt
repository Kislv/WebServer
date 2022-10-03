cmake_minimum_required(VERSION 3.10)

ADD_DEFINITIONS ( -levent_core )

# set the project name
project(WebServer)

# add the executable
include_directories(include)

add_executable(WebServer main.cpp src/http.cpp src/file.cpp src/threadPool.cpp src/job.cpp)
