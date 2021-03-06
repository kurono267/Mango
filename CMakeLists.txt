cmake_minimum_required (VERSION 3.7.2)
# Mango render library and examples
project (MANGO)

add_definitions(-std=c++17)

link_directories(/usr/local/opt/llvm/lib)

if(UNIX)
if (NOT APPLE)
    find_package(PkgConfig REQUIRED)
endif()
endif()
find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED)
find_package(glm REQUIRED)

set(ENTT_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/external/entt/single_include)

set(MANGO_INCLUDE ${GLFW_INCLUDE_DIRS} ${GLM_INCLUDE_DIRS} ${VULKAN_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/external ${CMAKE_CURRENT_SOURCE_DIR}/src ${Vulkan_INCLUDE_DIR} ${ENTT_INCLUDE})
message(${CMAKE_CURRENT_SOURCE_DIR})
link_directories(${CMAKE_SOURCE_DIR}/bin)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
link_directories($ENV{VULKAN_SDK}/lib)

add_subdirectory(src)
#add_subdirectory(examples)
add_subdirectory(tests)