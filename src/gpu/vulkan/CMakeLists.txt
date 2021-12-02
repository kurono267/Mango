# Vulkan gpu based
set(GPU_VULKAN
        VulkanInit.hpp VulkanInit.cpp)

add_library(gpu_vulkan ${GPU_VULKAN})
target_include_directories(gpu_vulkan PUBLIC ${Vulkan_INCLUDE_DIR})
target_link_libraries(gpu_vulkan gpu_general ${Vulkan_LIBRARIES} stdc++)