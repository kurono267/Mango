# Metal gpu based
set(MANGO_METAL
        MetalInit.hpp MetalInit.mm)

add_library(mango_metal ${MANGO_METAL})
target_include_directories(mango_metal PRIVATE)
target_link_libraries(mango_metal stdc++ "-framework Metal")