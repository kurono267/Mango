#version 450
#extension GL_ARB_separate_shader_objects : enable
//#extension GL_GOOGLE_include_directive : enable

#include "light.glsl"

layout(binding = 0) uniform sampler2D posTex;
layout(binding = 1) uniform sampler2D normalTex;
layout(binding = 2) uniform sampler2D albedoTex;
layout(binding = 3) uniform sampler2D material;

layout(binding = 4) uniform CameraPos {
    vec3 pos;
} camera;

layout(location = 0) in vec2 uv;

layout(location = 0) out vec4 fragColor;

void main() {
    vec3 pos = texture(posTex,uv).xyz;
    vec3 normal = normalize(texture(normalTex,uv).xyz*2.0f-1.0f);
    vec3 lightDir = normalize(vec3(1.0f,-1.0f,1.f));
    vec3 wo = normalize(camera.pos-pos);

    vec2 rm = texture(material,uv).rg;
    vec3 albedo = texture(albedoTex,uv).rgb;

    vec3 f = light(lightDir,wo,normal,rm.y,rm.x,albedo)*10.0f;

    fragColor = vec4(f,1.f);//vec4(vec3(uv,0.0), 1.0);
}