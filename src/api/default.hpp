//
// Created by kurono267 on 08.03.18.
//

#pragma once

#define GLM_FORCE_RADIANS
#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_XYZW_ONLY

#ifndef MANGO_FRAMEWORK
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include <glm/gtc/matrix_transform.hpp> // glm::perspective
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#else
#include <mango/external/glm/glm.hpp>
#include <mango/external/glm/gtc/type_ptr.hpp> // glm::value_ptr
#include <mango/external/glm/gtc/matrix_transform.hpp> // glm::perspective
#include <mango/external/glm/gtx/rotate_vector.hpp>
#include <mango/external/glm/gtc/quaternion.hpp>
#include <mango/external/glm/gtx/quaternion.hpp>
#include <mango/external/glm/gtx/string_cast.hpp>
#endif

#include <string>
#include <memory>

