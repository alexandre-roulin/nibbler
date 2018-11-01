cmake_minimum_required(VERSION 3.12)
project(asio)

set(CMAKE_CXX_STANDARD 14)

add_executable(asio
        srcs/timer_sync.cpp srcs/timer_async.cpp srcs/class.cpp srcs/tcp_client.cpp srcs/tcp_server.cpp srcs/server_async.cpp)
