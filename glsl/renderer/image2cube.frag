#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 uv;

layout(location = 0) out vec4 outColor;

layout(binding = 1) uniform sampler2D image;

const vec2 invAtan = vec2(0.1591, 0.3183);
vec2 SampleSphericalMap(vec3 v){
    vec2 uv = vec2(atan(v.y, v.x), -asin(v.z));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}

void main() {
    float exposure = 1.0f;
    vec3 hdrColor = texture(image,SampleSphericalMap(normalize(pos))).xyz;
    vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);
    outColor = vec4(clamp(mapped,0.f,1.f),1.0);
}
