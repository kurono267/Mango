#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 1, binding = 0) uniform sampler2D atlas;

layout(location = 0) in vec2 uv;
layout(location = 1) in vec4 color;

layout(location = 0) out vec4 outColor;

void main() {
    vec4 tex = vec4(1.f,0.f,0.f,1.f);
    float a = 1.0f;
    outColor = vec4(color.rgb*tex.rgb,a);
}