cmake_minimum_required(VERSION 4.0)
set(PROJECT_NAME http_server)

project(${PROJECT_NAME} C)

set(CMAKE_C_STANDARD 23)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror -g")

add_executable(${PROJECT_NAME} src/main.c src/map.c)
