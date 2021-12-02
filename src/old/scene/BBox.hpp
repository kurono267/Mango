//
// Created by kurono267 on 2018-12-14.
//

#pragma once

#include "../api/default.hpp"
#include <iostream>

/**
 * Bounding Box
 */
struct BBox {
    BBox();
    BBox(const glm::vec3& min, const glm::vec3& max);
    BBox(const BBox& a);

    void setMin(const glm::vec3& min);
    void setMax(const glm::vec3& max);

    glm::vec3 getMin() const;
    glm::vec3 getMax() const;

    glm::vec3 getSize() const;

	/**
	 * Expand bounding box with bounding box
	 * @param min - min of box
	 * @param max - max of box
	 */
    void expand(const glm::vec3& min,const glm::vec3& max);
    /**
     * Expand bounding box with bounding box
     * @param box - bounding box
     */
	void expand(const BBox& box);
	/**
	 * Expand bounding box with point
	 * @param p - point
	 */
    void expand(const glm::vec3& p);
	/**
	 * Compute maximal dimension in bounding box
	 * @return 0 - X Axis, 1 - Y Axis, 2 - Z Axis
	 */
    uint32_t maxDim();
	/**
	 * Apply transform to bounding box and return result
	 * @param transform - transform
	 * @return bounding box after apply transform
	 */
    BBox applyTransform(const glm::mat4& transform);
    /**
     * Get list of points of bounding box
     * @return list of points of bounding box
     */
    std::vector<glm::vec3> points() const;

    union {
        glm::vec4 bounds[2];
        struct {
            glm::vec4 min;
            glm::vec4 max;
        };
    };
};

std::ostream& operator<<(std::ostream& stream, const BBox& box);
