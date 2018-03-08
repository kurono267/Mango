set(MANGO_SRC
app/MainApp.cpp
)

set(MANGO_VK
vk/instance_vk.cpp
vk/device_vk.cpp
vk/swapchain.cpp
)

add_library(mango SHARED ${MANGO_SRC} ${MANGO_VK})
target_link_libraries(mango glfw vulkan)