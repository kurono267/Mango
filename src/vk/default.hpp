//
// Created by kurono267 on 05.06.18.
//

#ifndef MANGO_VK_DEFAULT_HPP
#define MANGO_VK_DEFAULT_HPP

#include <TargetConditionals.h> // TODO add ifdef for MacOS only

#ifndef TARGET_OS_IOS
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#endif
#include <vulkan/vulkan.hpp>
#ifdef TARGET_OS_IOS
#include <vulkan/vulkan_ios.h>
#endif

#define GLM_FORCE_RADIANS
#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_XYZW_ONLY
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include <glm/gtc/matrix_transform.hpp> // glm::perspective
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#endif //MANGO_DEFAULT_HPP
