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

layout(binding = 5) uniform samplerCube filtredIBL;
layout(binding = 6) uniform sampler2D specBRDF;
layout(binding = 7) uniform samplerCube irradianceTex;

layout(location = 0) in vec2 uv;

layout(location = 0) out vec4 fragColor;

vec3 ibl(vec3 N,vec3 V, vec3 pos, vec3 albedo, float metallic, float roughness, float lightScale){
    vec3 R = reflect(-V, N);

    vec3 F0 = vec3(0.04f);
    F0      = mix(F0, albedo, metallic);

    vec3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);

    vec3 kS = F;
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - metallic;

    const float MAX_REFLECTION_LOD = 5.0;
    vec3 irradiance = texture(irradianceTex, N).rgb*lightScale;
    vec3 diffuse    = irradiance * albedo;

    vec3 prefilteredColor = textureLod(filtredIBL, R,  roughness * MAX_REFLECTION_LOD).rgb*lightScale;
    vec2 envBRDF  = texture(specBRDF, vec2(max(dot(N, V), 0.0), roughness)).rg;
    vec3 specular = prefilteredColor * (F * envBRDF.x + envBRDF.y);

    vec3 ambient = (kD * diffuse + specular);

    return ambient;
}

void main() {
    vec4 posData = texture(posTex,uv);
    vec3 pos = posData.xyz;
    vec3 normal = normalize(texture(normalTex,uv).xyz*2.0f-1.0f);
    vec3 lightDir = normalize(vec3(1.0f,-1.0f,1.f));
    vec3 wo = normalize(camera.pos-pos);

    vec2 rm = texture(material,uv).rg;
    vec4 albedo = texture(albedoTex,uv);

    vec3 f = ibl(normal,wo,pos,albedo.rgb,rm.y,rm.x,2.0f);

    //vec3 f = light(lightDir,wo,normal,rm.y,rm.x,albedo)*10.0f;

    fragColor = vec4(f*posData.a,posData.a);//vec4(vec3(uv,0.0), 1.0);
}