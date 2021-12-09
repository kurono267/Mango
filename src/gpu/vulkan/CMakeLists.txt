# Vulkan gpu based
set(GPU_VULKAN
        convert/VkTextureTypes.cpp convert/VkTextureTypes.hpp
        convert/VkRenderPipelineTypes.cpp convert/VkRenderPipelineTypes.hpp
        VulkanInit.hpp VulkanInit.cpp
        VulkanResources.cpp VulkanResources.hpp)

add_library(gpu_vulkan ${GPU_VULKAN})
target_include_directories(gpu_vulkan PUBLIC ${Vulkan_INCLUDE_DIR})
target_link_libraries(gpu_vulkan gpu_general ${Vulkan_LIBRARIES} stdc++)