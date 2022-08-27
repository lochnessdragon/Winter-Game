cmake_minimum_required(VERSION 3.2)

project(Glad)

add_library(Glad STATIC src/glad.c)
include_directories(include/)
