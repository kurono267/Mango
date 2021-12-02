# Vulkan gpu based
set(MANGO_VULKAN
        VulkanInit.hpp VulkanInit.cpp)

add_library(mango_vulkan ${MANGO_VULKAN})
target_include_directories(mango_vulkan PRIVATE ${Vulkan_INCLUDE_DIR})
target_link_libraries(mango_vulkan ${Vulkan_LIBRARIES} stdc++)