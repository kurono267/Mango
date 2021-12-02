# Vulkan gpu based
set(GPU_VULKAN
        utils/VkTextureTypes.cpp utils/VkTextureTypes.hpp
        utils/VkRenderPipelineTypes.cpp utils/VkRenderPipelineTypes.hpp
        VulkanInit.hpp VulkanInit.cpp
        VulkanResources.cpp VulkanResources.hpp)

add_library(gpu_vulkan ${GPU_VULKAN})
target_include_directories(gpu_vulkan PUBLIC ${Vulkan_INCLUDE_DIR})
target_link_libraries(gpu_vulkan gpu_general ${Vulkan_LIBRARIES} stdc++)