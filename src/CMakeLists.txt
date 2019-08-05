set(MANGO_SRC
app/MainApp.cpp
        api/Pipeline.cpp
        api/Buffer.hpp
        unified/Mesh.cpp
        unified/Mesh.hpp
        api/Texture.hpp
        api/Framebuffer.hpp
        api/Pipeline.cpp
        api/Framebuffer.cpp
        api/Utils.hpp
        api/Utils.cpp
        api/CommandBuffer.hpp
        unified/Utils.cpp
        unified/Utils.hpp
scene/Camera.cpp
scene/Camera.hpp
        unified/Image.cpp
        unified/Uniform.cpp
        unified/Uniform.hpp scene/SceneTransform.cpp scene/SceneTransform.hpp scene/Scene.cpp scene/Scene.hpp scene/Material.cpp scene/Material.hpp scene/Assets.cpp scene/Assets.hpp)

set(MANGO_VK
        vk/InstanceVK.cpp
        vk/DeviceVK.cpp
        vk/Swapchain.cpp
        vk/PipelineVK.cpp
        vk/PipelineVK.hpp
        vk/ConvertVK.hpp
        vk/ConvertVK.cpp
        vk/BufferVK.cpp
        vk/BufferVK.hpp
        vk/TextureVK.cpp
        vk/TextureVK.hpp
        vk/FramebufferVK.cpp
        vk/FramebufferVK.hpp
        vk/CommandBufferVK.cpp
        vk/CommandBufferVK.hpp
        api/DescSet.hpp
        vk/DescSetVK.cpp
        vk/DescSetVK.hpp
        scene/SceneNode.cpp scene/SceneNode.hpp scene/Geometry.cpp scene/Geometry.hpp)

add_definitions(-DMVK_CONFIG_FULL_IMAGE_VIEW_SWIZZLE=1)

add_library(mango SHARED ${MANGO_SRC} ${MANGO_VK})
target_link_libraries(mango glfw ${Vulkan_LIBRARY} shaderc_combined libc++fs.a)