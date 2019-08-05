#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 1,binding = 0) uniform sampler2D albedo;
layout(set = 1,binding = 1) uniform sampler2D roughness;
layout(set = 1,binding = 2) uniform sampler2D metalness;

layout(location = 0) in vec2 uv;
layout(location = 0) out vec4 fragColor;

void main() {
    fragColor = texture(albedo,uv);
}