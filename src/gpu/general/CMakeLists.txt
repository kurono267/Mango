# General GPU
set(GPU_GENERAL
        BufferRaw.cpp BufferRaw.hpp
        RenderPipelineTypes.hpp
        Texture.hpp Texture.cpp
        TextureRaw.hpp TextureRaw.cpp TextureTypes.hpp)

add_library(gpu_general ${GPU_GENERAL})
target_include_directories(gpu_general PRIVATE ${MANGO_PATH} ${GLM_INCLUDE})
target_link_libraries(gpu_general stdc++)