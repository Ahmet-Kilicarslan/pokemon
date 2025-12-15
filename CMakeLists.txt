cmake_minimum_required(VERSION 4.0)
project(pokemon C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror")

add_executable(project
        main.c
        structs.h
        functions.h
        game.c
        initialize.c
        damage.c)
