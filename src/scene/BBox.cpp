//
// Created by kurono267 on 2018-12-14.
//

#include "BBox.hpp"
#include <vector>

BBox::BBox() : min(std::numeric_limits<float>::infinity()), max(-std::numeric_limits<float>::infinity()) {}
BBox::BBox(const BBox& a) : min(a.min), max(a.max) {}
BBox::BBox(const glm::vec3& _min, const glm::vec3& _max) : min(_min,1.0f),max(_max,1.f) {

}

void BBox::expand(const glm::vec3& _min,const glm::vec3& _max){
    if(isfinite(_min.x) && isfinite(_min.y) && isfinite(_min.z))expand(_min);
	if(isfinite(_max.x) && isfinite(_max.y) && isfinite(_max.z))expand(_max);
}
void BBox::expand(const glm::vec3& _p){
    min = glm::min(glm::vec4(_p,1.f),min);
    max = glm::max(glm::vec4(_p,1.f),max);
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

std::vector<glm::vec3> BBox::points() const {
	std::vector<glm::vec3> points(8);
	for(int p = 0;p<8;++p) {
		int i = p/4;
		int t0 = p%4;
		int j = t0/2;
		int k = t0%2;
		points[p] = glm::vec3(bounds[i].x, bounds[j].y, bounds[k].z);
	}
	return points;
}

std::ostream& operator<<(std::ostream& stream, const BBox& box){
	stream << "BBox min " << glm::to_string(box.min) << " max " << glm::to_string(box.max);
	return stream;
}

void BBox::setMin(const glm::vec3& _min) {
	min = glm::vec4(_min,1.0f);
}

void BBox::setMax(const glm::vec3& _max) {
	max = glm::vec4(_max,1.0f);
}

glm::vec3 BBox::getMin() const {
	return glm::vec3(min);
}

glm::vec3 BBox::getMax() const {
	return glm::vec3(max);
}
