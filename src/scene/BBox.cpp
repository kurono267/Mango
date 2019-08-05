//
// Created by kurono267 on 2018-12-14.
//

#include "BBox.hpp"
#include <vector>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

BBox::BBox() : min(std::numeric_limits<float>::infinity()), max(-std::numeric_limits<float>::infinity()) {}
BBox::BBox(const BBox& a) : min(a.min), max(a.max) {}

void BBox::expand(const glm::vec3& _min,const glm::vec3& _max){
    expand(_min);
    expand(_max);
}
void BBox::expand(const glm::vec3& _p){
    min = glm::min(_p,min);
    max = glm::max(_p,max);
}

void BBox::expand(const BBox &box) {
	expand(box.min,box.max);
}

uint32_t BBox::maxDim(){
    glm::vec3 axisSize = max-min;
    if(axisSize.x <= axisSize.y){
        if(axisSize.y >= axisSize.z)return 1; // Y axis
    } else {
        if(axisSize.x >= axisSize.z)return 0; // X axis
    }
    return 2; // Z axis Z more then other
}

BBox BBox::applyTransform(const glm::mat4 &transform) {
	glm::vec3 bounds[2] = {min,max};
	BBox box;
	for(int p = 0;p<8;++p) {
		int i = p/4;
		int t0 = p%4;
		int j = t0/2;
		int k = t0%2;
		glm::vec4 pos = transform*glm::vec4(bounds[i].x, bounds[j].y, bounds[k].z,1.f);
		box.expand(pos);
	}
	return box;
}

std::ostream& operator<<(std::ostream& stream, const BBox& box){
	stream << "BBox min " << glm::to_string(box.min) << " max " << glm::to_string(box.max);
	return stream;
}
