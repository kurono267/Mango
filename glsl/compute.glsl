#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_GOOGLE_include_directive : enable

layout (local_size_x = 16, local_size_y = 16) in;

layout (set=0,binding = 0, rgba32f) uniform image2D inputImage;
layout (set=0,binding = 1, rgba16) uniform image2D resultImage;

const ivec2 size = ivec2(1280,720);

void main() {
    ivec2 pixelCoord = ivec2(gl_GlobalInvocationID.xy);
    if(pixelCoord.x >= size.x || pixelCoord.y >= size.y)return;

    vec4 color = vec4(0.f);
    float norm = 0.f;
    for(int sy = -5;sy<=5;++sy){
        int y = pixelCoord.y+sy;
        if(y >= size.y || y < 0)continue;
        for(int sx = -5;sx<=5;++sx){
            int x = pixelCoord.x+sx;
            if(x >= size.x || x < 0)continue;
            color += imageLoad(inputImage,ivec2(x,y));
            norm += 1.f;
        }
    }
    if(norm != 0){
        color /= norm;
    }

    imageStore(resultImage,pixelCoord,color);
}
