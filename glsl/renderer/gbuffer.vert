#version 450
#extension GL_ARB_separate_shader_objects : enable

out gl_PerVertex {
    vec4 gl_Position;
};

layout(set = 0, binding = 0) uniform Camera {
    mat4 view;
    mat4 proj;
    mat4 viewProj;
} camera;

layout(set = 0, binding = 1) uniform Node {
    mat4 world;
    uint material;
} node;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inUV;

layout(location = 0) out vec3 outPos;
layout(location = 1) out vec3 outNormal;
layout(location = 2) out vec2 outUV;

void main() {
    vec4 worldPos = node.world*vec4(inPosition,1.f);
    gl_Position = camera.viewProj*worldPos;
    outUV = inUV;
    outPos = worldPos.xyz;
    outNormal = inNormal;
}