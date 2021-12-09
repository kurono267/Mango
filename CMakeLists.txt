cmake_minimum_required (VERSION 3.7.2)
# Mango render library and examples
project (Mango)

add_definitions(-std=c++17)

link_directories(/usr/local/opt/llvm/lib)

if(UNIX)
if (NOT APPLE)
    find_package(PkgConfig REQUIRED)
endif()
endif()
find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED)

set(MANGO_PATH ${CMAKE_SOURCE_DIR}/src)
set(GLM_INCLUDE ${CMAKE_SOURCE_DIR}/external/glm)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
link_directories(${CMAKE_SOURCE_DIR}/bin)

link_directories($ENV{VULKAN_SDK}/lib)

add_subdirectory(src)
add_subdirectory(examples)
add_subdirectory(tests)