cmake_minimum_required (VERSION 3.7.2)
# Mango render library and examples
project (MANGO)

add_definitions(-std=c++17)

find_package(PkgConfig REQUIRED)
find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED)
find_package(glm REQUIRED)
include_directories(${GLFW_INCLUDE_DIRS} ${GLM_INCLUDE_DIRS} ${VULKAN_INCLUDE_DIRS})
link_directories(${PROJECT_SOURCE_DIR}/bin)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
include_directories(${CMAKE_SOURCE_DIR}/src)

add_subdirectory(src)
add_subdirectory(examples)