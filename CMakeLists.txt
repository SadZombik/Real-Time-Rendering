cmake_minimum_required(VERSION 3.5)

project(Rendering)

file(GLOB SOURCE
    "src/*.c"
)

add_executable(main ${SOURCE})
