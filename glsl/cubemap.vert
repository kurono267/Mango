#version 450
#extension GL_ARB_separate_shader_objects : enable

out gl_PerVertex {
    vec4 gl_Position;
};

layout(binding = 0) uniform Camera {
    mat4 view;
    mat4 proj;
    mat4 viewProj;
} camera;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inUV;
layout(location = 0) out vec2 outUV;
layout(location = 1) out vec3 outPos;

void main() {
    vec4 pos = camera.viewProj*vec4(inPosition*5000.f,1.0);
    gl_Position = pos;
    outPos = inPosition;
    outUV = inUV;
}
