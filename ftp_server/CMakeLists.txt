cmake_minimum_required(VERSION 3.10)
project(FTPServer)

set(CMAKE_CXX_STANDARD 17)  # Change to 17

include_directories(include)

add_executable(ftp_server src/ftp_server.cpp src/ftp_utils.cpp)
