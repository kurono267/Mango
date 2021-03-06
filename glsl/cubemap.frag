#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec2 uv;
layout(location = 1) in vec3 pos;

layout(location = 0) out vec4 outColor;

layout(binding = 1) uniform samplerCube cubeMap;

void main() {
    outColor = vec4(clamp(textureLod(cubeMap,normalize(pos),0).xyz,0.f,1.f),1.0); // Simple gamma
}
