#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_GOOGLE_include_directive : enable

layout (local_size_x = 16, local_size_y = 16) in;

#define MAX_STACK 128
#define MAX_DIST  100000.0
#define RAY_MIN   0.01f

struct Vertex {
    vec4 pos;
    vec4 normal;
    vec4 uv;
    vec4 tangent;
    vec4 binormal;
};

struct Node {
    vec4  min;
    vec4  max;
    ivec4 data;
};

// Vertex Buffer
layout (set=0,binding=0,std430) buffer vertexData {
    readonly Vertex vertexes[];
};
// Index Buffer
layout (set=0,binding=1,std430) buffer indexData {
    readonly uint indexes[];
};

layout (set=0,binding = 2,std430) buffer nodeData {
    readonly Node nodes[];
};

layout (set=1,binding = 0, rgba32f) uniform image2D rayOrg;
layout (set=1,binding = 1, rgba32f) uniform image2D rayDir;

layout (set=2,binding = 0, rgba16) uniform image2D outputAlbedo;
layout (set=2,binding = 1, rgba16) uniform image2D outputNormal;
layout (set=2,binding = 2, rgba32f) uniform image2D outputPos;
layout (set=2,binding = 3, rgba16) uniform image2D outputMaterial;

layout(set=3,binding=0) uniform sampler2D materialAlbedo;
layout(set=3,binding=1) uniform sampler2D materialData;

const ivec2 size = ivec2(1280,720);

struct Ray {
    vec3 org;
    vec3 dir;
};

Ray createRay(vec3 org,vec3 dir){
    return Ray(org,dir);
}

struct RayHit {
    float tmax;
    float tmin;
    bool  status;

    vec3 bc;
    int  id1;
};

RayHit check(Node box,Ray ray){
    vec3 inv_dir = 1.0/ray.dir;

    vec3 lov = inv_dir*(box.min.xyz - ray.org);
    vec3 hiv = inv_dir*(box.max.xyz - ray.org);

    vec3 max_v = max(lov, hiv);
    vec3 min_v = min(lov, hiv);

    float tmin  = max(min_v.x,max(min_v.y,min_v.z));//reduce_max(min(lov, hiv));
    float tmax =  min(max_v.x,min(max_v.y,max_v.z));

    RayHit r;
    r.tmin = MAX_DIST;
    r.tmax = tmax;
    r.status = false;

    if(tmin <= tmax && (tmax > RAY_MIN)){
        r.tmin   = tmin>0.0f?tmin:tmax;
        r.status = true;
        return r;
    }
    return r;
}

RayHit intersect(const Ray ray, const vec3 v0, const vec3 v1, const vec3 v2) {
    vec3 e1 = v1 - v0;
    vec3 e2 = v2 - v0;
    // Calculate planes normal vector
    vec3 pvec = cross(ray.dir, e2);
    float det = dot(e1, pvec);

    RayHit hit;
    hit.status = false;
    hit.tmin = MAX_DIST;
    hit.bc = vec3(-1.f);

    // Ray is parallel to plane
    if (abs(det) < 1e-8) {
        return hit;
    }

    float inv_det = 1 / det;
    vec3 tvec = ray.org - v0;
    float u = dot(tvec, pvec) * inv_det;
    if (u < 0.0f || u > 1.0f) {
        return hit;
    }

    vec3 qvec = cross(tvec, e1);
    float v = dot(ray.dir, qvec) * inv_det;
    if (v < 0.0f || u + v > 1.0f) {
        return hit;
    }
    hit.tmin = dot(e2, qvec) * inv_det;
    hit.status = true;
    hit.bc = vec3(1.0f-u-v,u,v);
    return hit;
}

RayHit trace(Ray ray,float currDepth){
    // Check root, if not return
    RayHit hit = check(nodes[0],ray);
    if(!hit.status)return hit;

    hit.status = false;
    hit.tmin   = currDepth;

    int stack_nodes[MAX_STACK];
    int stack_top;

    stack_nodes[0] = 0;
    stack_top = 0;

    RayHit test;Node curr;
    int currId;
    // Max steps for debug
    uint step = 0;
    while(stack_top >= 0/* && step < 100*/){
        step++;
        // Get Top element from stack
        currId = stack_nodes[stack_top--];
        curr  = nodes[currId];

        if(curr.data.w != 0){ // Leaf
            // Check triangles
            test = check(curr,ray);
            if(test.tmin < hit.tmin){
                ivec4 triIds = curr.data;
                for(int i = 0;i<4;++i){
                    int triId = triIds[i];
                    if(triId >= 0){
                        Vertex vp0 = vertexes[indexes[triId+0]];Vertex vp1 = vertexes[indexes[triId+1]];Vertex vp2 = vertexes[indexes[triId+2]];
                        test = intersect(ray,vp0.pos.xyz,vp1.pos.xyz,vp2.pos.xyz);
                        if(test.tmin < hit.tmin){
                            hit = test;
                            hit.id1 = triId;
                            //break;
                        }
                    }
                }
            }
        } else {
            // Check left and right child
            int leftID  = curr.data.x;
            int rightID = curr.data.y;

            RayHit left = check(nodes[leftID],ray);
            RayHit right = check(nodes[rightID],ray);

            bool leftStatus  = left.tmin > 0.0f && left.tmin < hit.tmin;
            bool rightStatus = right.tmin > 0.0f && right.tmin < hit.tmin;

            if(leftStatus && rightStatus){
                // Left child always neares, if not swap
                if(right.tmin < left.tmin){
                    int tmp = leftID;
                    leftID  = rightID;
                    rightID = tmp;
                }
                if(stack_top < MAX_STACK-1)stack_nodes[++stack_top] = rightID;
                if(stack_top < MAX_STACK-1)stack_nodes[++stack_top] = leftID;
            } else if(leftStatus){
                if(stack_top < MAX_STACK-1)stack_nodes[++stack_top] = leftID;
            } else if(rightStatus){
                if(stack_top < MAX_STACK-1)stack_nodes[++stack_top] = rightID;
            }
        }

    }
    return hit;
}

struct Point {
    vec3 pos;
    vec3 normal;
    vec2 uv;
};

Point postIntersect(RayHit hit,Ray ray){
    int triId = hit.id1;
    Vertex vp0 = vertexes[indexes[triId+0]];Vertex vp1 = vertexes[indexes[triId+1]];Vertex vp2 = vertexes[indexes[triId+2]];
    vec3 n0 = vp0.normal.xyz; vec3 n1 = vp1.normal.xyz; vec3 n2 = vp2.normal.xyz;
    vec2 u0 = vp0.uv.xy; vec2 u1 = vp1.uv.xy; vec2 u2 = vp2.uv.xy;
    Point p;
    p.pos    = ray.org+ray.dir*hit.tmin;
    p.normal = normalize(hit.bc.x*n0+hit.bc.y*n1+hit.bc.z*n2);
    if(dot(-ray.dir,p.normal) < 0.0f){
        p.normal = -p.normal;
    }
    p.uv     = hit.bc.x*u0+hit.bc.y*u1+hit.bc.z*u2;
    return p;
}

void main() {
    ivec2 pixelCoord = ivec2(gl_GlobalInvocationID.xy);
    if(pixelCoord.x >= size.x || pixelCoord.y >= size.y)return;

    vec4 org = imageLoad(rayOrg,pixelCoord);
    vec4 dir = imageLoad(rayDir,pixelCoord);

    float depth = imageLoad(outputPos,pixelCoord).w;

    Ray ray = createRay(vec3(org),vec3(dir));

    RayHit hit = trace(ray,depth);
    vec4 pos = vec4(0.f);
    vec4 normal = vec4(0.f);
    vec4 albedo = vec4(0.f);
    vec4 material = vec4(0.f);
    if(hit.status){
        Point p = postIntersect(hit,ray);
        normal = vec4(p.normal,1.f);
        pos = vec4(p.pos,hit.tmin);
        albedo = texture(materialAlbedo,p.uv);
        material = texture(materialData,p.uv);

        imageStore(outputPos,pixelCoord,pos);
        imageStore(outputNormal,pixelCoord,normal);
        imageStore(outputAlbedo,pixelCoord,albedo);
        imageStore(outputMaterial,pixelCoord,material);
    }
}
