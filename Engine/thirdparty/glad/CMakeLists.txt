cmake_minimum_required(VERSION 3.10)

project(glad-loader LANGUAGES C)

set(SOURCES
    src/glad.c
)

add_library(glad ${SOURCES})
target_include_directories(glad PUBLIC include/)
