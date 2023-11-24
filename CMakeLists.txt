cmake_minimum_required (VERSION 3.8)
find_package(OpenGL REQUIRED)

set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

project ("Lab4")

file(GLOB shader_source_files RELATIVE shaders/ "*.frag" "*.vert")

add_custom_command(
	OUTPUT shaders.h
	COMMAND python ${PROJECT_SOURCE_DIR}/scripts/gen_shaders_header.py ${PROJECT_SOURCE_DIR}/shaders/ ${PROJECT_SOURCE_DIR}/shaders.h
	DEPENDS ${shader_source_files}
)

add_executable (Lab4
	"main.cpp" 
	"shader_utils.h"
	"shader_utils.cpp"
	"Torus.h"
	"Torus.cpp"
	"Axis3D.h" 
	"Axis3D.cpp"
	"LightCube.h"
	"LightCube.cpp"
	"Entity3D.h"
	"Entity3D.cpp"
	"Shader.h"
	"ScalarAnimator.h"
	"ScalarAnimator.cpp"
	"shaders.h"
	"App.h"
	"App.cpp" 
	"ColorAnimator.h"
	"ColorAnimator.cpp" 
	"time_utils.h" 
	"matrix_utils.h" 
	"rotation.h" 
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Lab4 PROPERTY CXX_STANDARD 20)
endif()

set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_EXAMPLES OFF)

add_subdirectory (external)
include_directories(
	external/glfw/include/
	external/glew/include/
	external/glm/
	external/freetype/include
	external/stb_image/
)

add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS
)

set(CMAKE_MODULE_LINKER_FLAGS "/machine:X64")
set(CMAKE_EXE_LINKER_FLAGS "/machine:X64")

target_link_libraries(Lab4 glfw GLEW_1130 glm)

configure_file("${CMAKE_SOURCE_DIR}/textures/iron.jpg" "${CMAKE_BINARY_DIR}/iron.jpg" COPYONLY)
