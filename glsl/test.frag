#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform Test {
    vec4 color;
} test;

layout(binding = 1) uniform sampler2D tex;

layout(location = 0) in vec2 uv;

layout(location = 0) out vec4 fragColor;

void main() {
    fragColor = texture(tex,uv);//vec4(vec3(uv,0.0), 1.0);
}