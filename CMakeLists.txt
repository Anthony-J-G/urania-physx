cmake_minimum_required(VERSION 4.0)

project(UraniaEngine)


set(URANIA_SOURCE_FILES	
	src/engine/world.c
)

set(URANIA_API_FILES
	src/engine/world.h
    src/engine/types.h
)

add_library(urania ${URANIA_SOURCE_FILES} ${URANIA_API_FILES})
target_include_directories(urania
	PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)

set_target_properties(urania PROPERTIES
	C_STANDARD 90
    C_STANDARD_REQUIRED YES
    C_EXTENSIONS NO
)

message(STATUS "CMake C compiler: ${CMAKE_C_COMPILER_ID}")
message(STATUS "CMake C++ compiler: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "CMake system name: ${CMAKE_SYSTEM_NAME}")

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src/engine" PREFIX "src" FILES ${URANIA_SOURCE_FILES})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" PREFIX "include" FILES ${URANIA_API_FILES})