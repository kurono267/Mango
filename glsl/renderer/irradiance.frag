#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 uv;

layout(location = 0) out vec4 outColor;

layout(binding = 1) uniform samplerCube image;

const float PI = 3.1415926535;

void main() {
	vec3 normal = normalize(pos);

    vec3 irradiance = vec3(0.0);

	vec3 up    = vec3(0.0, 0.0, 1.0);
	vec3 right = cross(up, normal);
	up         = cross(normal, right);

	float sampleDelta = 0.1f;
	float nrSamples = 0.0;
	for(float phi = 0.0; phi < 2.0 * PI; phi += sampleDelta)
	{
	    for(float theta = 0.0; theta < 0.5 * PI; theta += sampleDelta)
	    {
	        // spherical to cartesian (in tangent space)
	        vec3 tangentSample = vec3(sin(theta) * cos(phi),  sin(theta) * sin(phi), cos(theta));
	        // tangent space to world
	        vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * normal;

	        irradiance += texture(image, sampleVec).rgb * cos(theta) * sin(theta);
	        nrSamples++;
	    }
	}
	irradiance = PI * irradiance * (1.0 / float(nrSamples));
	outColor = vec4(irradiance,1.0f);
}
