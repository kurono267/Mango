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
#ifndef MANGO_FRAMEWORK
#include <vulkan/vulkan.hpp>
#ifdef TARGET_OS_IOS
#include <vulkan/vulkan_ios.h>
#endif
#else
#include <mango/external/vulkan/vulkan.hpp>
#ifdef TARGET_OS_IOS
#include <mango/external/vulkan/vulkan_ios.h>
#endif
#endif

#include "../api/default.hpp"

#endif //MANGO_DEFAULT_HPP
