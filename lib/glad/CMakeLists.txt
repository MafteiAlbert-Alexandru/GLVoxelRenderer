cmake_minimum_required(VERSION 3.3)
project(Glad)

add_library(glad SHARED include/glad/glad.h src/glad.c)
target_include_directories(glad PUBLIC include/)