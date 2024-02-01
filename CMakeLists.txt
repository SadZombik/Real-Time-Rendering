cmake_minimum_required(VERSION 3.5)

project(Rendering)

set(all_targets 2Dtransform 3Dtransform)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_USE_HYBRID_HPG ON CACHE BOOL "" FORCE)

# ImGui
set(IMGUI_DIRECTORY dependencies/imgui)
set(IMGUI_SOURCE ${IMGUI_DIRECTORY}/imconfig.h
	${IMGUI_DIRECTORY}/imgui.h
	${IMGUI_DIRECTORY}/imgui.cpp
	${IMGUI_DIRECTORY}/imgui_draw.cpp
	${IMGUI_DIRECTORY}/imgui_internal.h
	${IMGUI_DIRECTORY}/imgui_widgets.cpp
	${IMGUI_DIRECTORY}/imstb_rectpack.h
	${IMGUI_DIRECTORY}/imstb_textedit.h
	${IMGUI_DIRECTORY}/imstb_truetype.h
    ${IMGUI_DIRECTORY}/imgui_tables.cpp
	${IMGUI_DIRECTORY}/imgui_demo.cpp
	${IMGUI_DIRECTORY}/backends/imgui_impl_glfw.cpp
	${IMGUI_DIRECTORY}/backends/imgui_impl_opengl3.cpp
)

add_subdirectory(dependencies/glad)
add_subdirectory(dependencies/glfw)
add_subdirectory(dependencies/stb_image)
add_subdirectory(dependencies/glm)

# file(GLOB SOURCE "src/*.cpp")

add_executable(2Dtransform 
	src/2Dtransform.cpp
	src/shader.cpp
	${IMGUI_SOURCE}
)

add_executable(3Dtransform 
	src/3Dtransform.cpp
	src/shader.cpp
	${IMGUI_SOURCE}
)

add_compile_definitions(RES_DIR=${CMAKE_CURRENT_SOURCE_DIR}/res)
foreach(target ${all_targets})
	target_link_libraries(${target} ${OPENGL_LIBRARIES} glad glfw stb_image glm)
	target_include_directories(${target} PRIVATE ${IMGUI_DIRECTORY})
endforeach()
