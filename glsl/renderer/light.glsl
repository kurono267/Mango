const float PI = 3.14159265;
// Oran Nayar Diffuse
float diffuse(vec3 no, vec3 wi, vec3 wo, float r){
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

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness) {
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}

vec3 Fresnel(float VdotH, vec3 F0){
    return F0 + (1.0 - F0) * pow(1.0 - clamp(VdotH,0.0,1.0), 5.0);
}

float chiGGX(float v)
{
    return v > 0 ? 1 : 0;
}

float PGeom(float cosThN,float alpha, float nv){
    float cosTh2 = clamp(cosThN*cosThN,0.0,1.0);
    float chi = chiGGX( cosThN / clamp(nv,0.f,1.f) );
    float tan2   = (1.0-cosTh2)/cosTh2;
    return (2.0*chi)/(1.0+sqrt(1.0+alpha*alpha*tan2));
}

float Distrib(float NH,float alpha2){
    float NH2 = clamp(NH*NH,0.0,1.0);
    float den = NH2*alpha2+(1.0-NH2);
    return chiGGX(NH)*alpha2 / (PI*den*den+0.01f);
}

float specular(vec3 no,vec3 wi,vec3 wo,vec3 H,float r){
    float NV = max(dot(no,wo),0.01f);
    float NL = max(dot(no,wi),0.01f);
    float NH = max(dot(no,H),0.01f);

    float r2 = r*r;
    float G = PGeom(NV,r2,max(dot(H,wi),0.01f))*PGeom(NL,r2,max(dot(H,wo),0.01f));
    float D = Distrib(NH,r2);

    return G*D*0.25/NV;
}

vec3 light(vec3 wi,vec3 wo,vec3 normal,float metallic,float r,vec3 albedo){
    vec3 N = normal;
    vec3 L = wi;
    vec3 V = wo;

    vec3 H = normalize(L+V);

    vec3 F0 = vec3(0.04f);
    F0      = mix(F0, albedo, metallic);//1.0-texture(diffuseTexture,uv).rgb;//vec3(1.0, 0.86, 0.56); // For testing
    float HV = max(dot(H,V),0.0);
    vec3 F = fresnelSchlickRoughness(HV,F0,r);

    float spec = specular(N,L,V,H,r);
    float diff = diffuse(N,L,V,r);

    return (max(spec*F,0.f)+diff*albedo*clamp((1.0f-F)*(1.0f-metallic),0.0,1.0));
}
