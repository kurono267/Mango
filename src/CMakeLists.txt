add_definitions(-DMVK_CONFIG_FULL_IMAGE_VIEW_SWIZZLE=1)

add_subdirectory(gpu/general)
add_subdirectory(gpu/vulkan)
add_subdirectory(gpu/metal)

add_library(mango SHARED
        gpu/GPUInit.hpp
        gpu/GPU.hpp
        gpu/GPU.cpp)
target_include_directories(mango PUBLIC ${MANGO_INCLUDE})
target_link_libraries(mango gpu_vulkan gpu_metal)