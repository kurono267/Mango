#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 beginPos;
layout(location = 1) in vec3 endPos;

layout(location = 0) out vec4 rayPos;
layout(location = 1) out vec4 rayDir;

void main() {
    rayPos = vec4(beginPos,1.f);
    rayDir = vec4(normalize(endPos-beginPos),1.f);
}