cmake_minimum_required(VERSION 3.27)
project(cumber C)

set(CMAKE_C_STANDARD 11)

add_executable(cumber main.c
        mnist.c
        mnist.h
        feedforward.c
        feedforward.h
)
