#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform sampler2D noiseTex;

layout(location = 0) in vec2 uv;

layout(location = 0) out vec4 fragColor;

float qunticCurve(float t) {
    return t * t * t * (t * (t * 6 - 15) + 10);
}

float perlinNoise2DOctave(vec2 coord,float amp, vec2 freq){
    vec2 noiseSize = textureSize(noiseTex,0);
    vec2 noiseCoord = fract(coord*freq);
    vec2 pixelSize = 1.0f/noiseSize;
    vec2 pixelCoord = noiseCoord*noiseSize;
    vec2 localCoord = fract(pixelCoord);

    float interpCoords[4];
    for(int ys = 0;ys<2;++ys){
        for(int xs = 0;xs<2;++xs){
            vec2 gradient = texelFetch(noiseTex,ivec2(floor(pixelCoord+vec2(xs,ys))),0).xy;
            vec2 distance = localCoord-vec2(xs,ys);
            interpCoords[ys*2+xs] = dot(distance,gradient);
        }
    }

    vec2 quanticCoords = vec2(qunticCurve(localCoord.x),qunticCurve(localCoord.y));

    float interpByX0 = mix(interpCoords[0],interpCoords[1],quanticCoords.x);
    float interpByX1 = mix(interpCoords[2],interpCoords[3],quanticCoords.x);

    float interpByY = mix(interpByX0,interpByX1,quanticCoords.y);
    return interpByY*amp;
}

float perlinNoise2D(vec2 coord,float amp, vec2 freq, float presistance, int octaves){
    float perlin = 0.f;
    for(int i = 0;i<octaves;++i){
        perlin += perlinNoise2DOctave(coord,amp,freq);
        freq *= 2;
        amp *= presistance;
    }
    return perlin;
}

void main() {
    float perlin = perlinNoise2D(uv,1.f,vec2(0.02f,0.02f),0.5f,8);

    fragColor = vec4(perlin,perlin,perlin,1.f);
}