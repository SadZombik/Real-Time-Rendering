cmake_minimum_required(VERSION 3.5)
project(Glad)

if (WIN32)
    add_library(glad include/glad/glad.h src/glad_win.c)
elseif(UNIX)
    add_library(glad include/glad/glad.h src/glad_linux.c)
endif()

target_include_directories(glad PUBLIC include/)
