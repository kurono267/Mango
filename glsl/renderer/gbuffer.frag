#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 1, binding = 0) uniform sampler2D albedo;
layout(set = 1, binding = 1) uniform sampler2D metallicRoughness;
layout(set = 1, binding = 2) uniform MaterialData {
    vec2 factors;
} matData;

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

layout(location = 0) out vec4 fragNormal;
layout(location = 1) out vec4 fragPos;
layout(location = 2) out vec4 fragDiffuse;
layout(location = 3) out vec4 fragMaterial;

void main() {
    fragDiffuse = texture(albedo,uv);
    fragPos = vec4(pos,1.f);
    fragNormal = vec4(normalize(normal)*0.5f+0.5f,1.f);
    fragMaterial = vec4(texture(metallicRoughness,uv).gr*matData.factors,0.f,0.f);
}