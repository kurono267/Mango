#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform sampler2D tex;

layout(location = 0) in vec2 uv;

layout(location = 0) out vec4 fragColor;

void main() {
    float p = uv.x*0.25f+0.2f;
    fragColor = vec4(p,p,p,1.f);//texture(tex,uv);//vec4(vec3(uv,0.0), 1.0);
}