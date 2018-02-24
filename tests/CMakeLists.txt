cmake_minimum_required (VERSION 3.0)

project (refrain2_unit_tests)

add_subdirectory (../../floral "${PROJECT_BINARY_DIR}/floral")
add_subdirectory (../../clover "${PROJECT_BINARY_DIR}/clover")
add_subdirectory (../../helich "${PROJECT_BINARY_DIR}/helich")
add_subdirectory (../../refrain2 "${PROJECT_BINARY_DIR}/refrain2")

include_directories("${PROJECT_SOURCE_DIR}/src")

# preprocessors
add_definitions (
	-DPLATFORM_WINDOWS
	-D_CRT_SECURE_NO_WARNINGS)

file(GLOB_RECURSE file_list
    "${PROJECT_SOURCE_DIR}/src/*.cpp")

add_executable(refrain2_unit_tests ${file_list})

target_link_libraries(refrain2_unit_tests
	clover
	floral
	helich
	refrain2
	comctl32)