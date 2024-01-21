cmake_minimum_required(VERSION 3.5)

project(Rendering)

set(TARGET main)

file(GLOB SOURCE
    "src/main.c"
)

add_executable(main ${SOURCE})
