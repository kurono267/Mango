set(MANGO_SRC
app/MainApp.cpp
        api/pipeline.cpp api/buffer.hpp)

set(MANGO_VK
vk/instance_vk.cpp
vk/device_vk.cpp
vk/swapchain.cpp
        api/pipeline.cpp vk/pipeline_vk.cpp vk/pipeline_vk.hpp vk/convert_vk.hpp vk/convert_vk.cpp vk/buffer_vk.cpp vk/buffer_vk.hpp)

add_library(mango SHARED ${MANGO_SRC} ${MANGO_VK})
target_link_libraries(mango glfw vulkan)