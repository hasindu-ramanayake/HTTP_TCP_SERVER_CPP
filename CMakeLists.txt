cmake_minimum_required(VERSION 3.16.3)

project(web_server)

add_executable(web_server main_server.cpp http_tcp_server.cpp)