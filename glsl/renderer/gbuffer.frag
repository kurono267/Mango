#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform sampler2D tex;

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;
layout(location = 3) flat in uint material;

layout(location = 0) out vec4 fragNormal;
layout(location = 1) out vec4 fragPos;
layout(location = 2) out vec4 fragDiffuse;

void main() {
    fragDiffuse = vec4(float(material)/3.f,0.f,0.f,1.f);//texture(tex,uv);
    fragPos = vec4(0.f,1.f,0.f,1.f);
    fragNormal = vec4(0.f,0.f,1.f,1.f);
}