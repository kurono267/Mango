#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform sampler2D pos;
layout(binding = 1) uniform sampler2D normal;
layout(binding = 2) uniform sampler2D albedo;
layout(binding = 3) uniform sampler2D material;

layout(location = 0) in vec2 uv;

layout(location = 0) out vec4 fragColor;

void main() {
    fragColor = vec4(texture(albedo,uv).rgb,1.f);//vec4(vec3(uv,0.0), 1.0);
}