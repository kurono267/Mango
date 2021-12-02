# Metal gpu based
set(GPU_METAL
        MetalInit.hpp MetalInit.mm)

add_library(gpu_metal ${GPU_METAL})
target_include_directories(gpu_metal PRIVATE)
target_link_libraries(gpu_metal gpu_general stdc++ "-framework Metal")