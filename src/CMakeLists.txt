set(MANGO_SRC
app/MainApp.cpp
api/pipeline.cpp
api/buffer.hpp
api/mesh.cpp
api/mesh.hpp
api/texture.hpp
api/framebuffer.hpp
api/pipeline.cpp
api/framebuffer.cpp
api/utils.hpp
api/utils.cpp
api/commandbuffer.hpp)

set(MANGO_VK
vk/instance_vk.cpp
vk/device_vk.cpp
vk/swapchain.cpp
        vk/pipeline_vk.cpp vk/pipeline_vk.hpp vk/convert_vk.hpp vk/convert_vk.cpp vk/buffer_vk.cpp vk/buffer_vk.hpp vk/texture_vk.cpp vk/texture_vk.hpp vk/framebuffer_vk.cpp vk/framebuffer_vk.hpp vk/commandbuffer_vk.cpp vk/commandbuffer_vk.hpp)

add_library(mango SHARED ${MANGO_SRC} ${MANGO_VK})
target_link_libraries(mango glfw vulkan shaderc_combined)