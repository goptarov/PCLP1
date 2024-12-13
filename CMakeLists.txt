cmake_minimum_required(VERSION 3.30)
project(tema1 C)

set(CMAKE_C_STANDARD 11)

add_executable(tema1 pachete.c)

target_link_libraries(tema1 m)