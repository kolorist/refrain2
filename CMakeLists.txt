cmake_minimum_required (VERSION 3.0)

project (refrain2)

include_directories("${PROJECT_SOURCE_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/include")

file(GLOB_RECURSE file_list
    "${PROJECT_SOURCE_DIR}/src/cu/*.cpp")

add_definitions (
	-D_CRT_SECURE_NO_WARNINGS
	-DPLATFORM_WINDOWS)

add_library(refrain2 ${file_list})

target_link_libraries (refrain2
	clover
	floral
	helich)

target_include_directories (refrain2 PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
