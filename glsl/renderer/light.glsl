const float PI = 3.14159265;
// Oran Nayar Diffuse
float diffuse(float r, vec3 wi, vec3 wo, vec3 no){
    float LdotV = dot(wi, wo);
    float NdotL = dot(wi, no);
    float NdotV = dot(no, wo);

    float s = LdotV - NdotL * NdotV;
    float t = mix(1.0, max(NdotL, NdotV), step(0.0, s));

    float sigma2 = r * r;
    float A = 1.0 - (0.5*sigma2) / (sigma2 + 0.33);
    float B = (0.45 * sigma2) / (sigma2 + 0.09);

    return max(0.0, NdotL) * (A + B * s / t) / PI;
}
