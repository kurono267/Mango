#version 450
#extension GL_ARB_separate_shader_objects : enable

out gl_PerVertex {
    vec4 gl_Position;
};

layout(set = 0, binding = 0) uniform Camera {
    mat4 world;
    mat4 invVP;
} camera;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inUV;

layout(location = 0) out vec3 beginPos;
layout(location = 1) out vec3 endPos;

void main() {
    gl_Position = vec4(inPosition,1.f);

    vec4 origin = camera.world*vec4(0.f,0.f,0.f,1.f);
    origin /= origin.w;

    beginPos = origin.xyz;

    vec4 endView = camera.invVP*vec4(inPosition.xy,1.f,1.f);
    vec4 endWorld = camera.world*endView;
    endPos = endWorld.xyz/endWorld.w;
}