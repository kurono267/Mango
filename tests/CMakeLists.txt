find_package(GTest REQUIRED)

add_executable(GPUInit GPU/VulkanTest.cpp GPU/VulkanTest.hpp GPU/MetalTest.cpp GPU/MetalTest.hpp GPU/main.cpp)
target_include_directories(GPUInit PRIVATE ${MANGO_PATH} ${GTEST_INCLUDE_DIRS})
target_link_libraries(GPUInit mango ${GTEST_LIBRARIES})

#add_executable(ShaderCodeTest ShaderCode/ShaderCodeTest.cpp)
#target_include_directories(ShaderCodeTest PUBLIC ${MANGO_INCLUDE} PRIVATE ${GTEST_INCLUDE_DIRS})
#target_link_libraries(ShaderCodeTest mango ${GTEST_LIBRARIES})
#
#add_custom_command(TARGET ShaderCodeTest PRE_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy
#        ${CMAKE_CURRENT_SOURCE_DIR}/ShaderCode/input.glsl $<TARGET_FILE_DIR:ShaderCodeTest>/tests/ShaderCodeTest/input.glsl)
#add_custom_command(TARGET ShaderCodeTest PRE_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy
#        ${CMAKE_CURRENT_SOURCE_DIR}/ShaderCode/mutable.glsl $<TARGET_FILE_DIR:ShaderCodeTest>/tests/ShaderCodeTest/mutable.glsl)
#add_custom_command(TARGET ShaderCodeTest PRE_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy
#        ${CMAKE_CURRENT_SOURCE_DIR}/ShaderCode/modified.glsl $<TARGET_FILE_DIR:ShaderCodeTest>/tests/ShaderCodeTest/modified.glsl)