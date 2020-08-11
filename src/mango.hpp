//
// Created by kurono267 on 08.03.18.
//

#ifndef MANGO_HPP
#define MANGO_HPP

#include "app/MainApp.hpp"
#include "app/BaseApp.hpp"
#include "api/Pipeline.hpp"
#include "api/Buffer.hpp"
#include "api/Device.hpp"
#include "api/Framebuffer.hpp"
#include "api/Instance.hpp"
#include "api/Pipeline.hpp"
#include "api/Texture.hpp"
#include "api/Utils.hpp"
#include "api/DescLayout.hpp"
#include "api/DescPool.hpp"
#include "api/DescSet.hpp"
#include "api/CommandBuffer.hpp"
#include "api/Utils.hpp"
#include "api/Compute.hpp"

#include "unified/Mesh.hpp"
#include "unified/Uniform.hpp"
#include "unified/Utils.hpp"
#include "unified/Image.hpp"
#include "unified/RenderTarget.hpp"
#include "unified/Timer.hpp"

#include "scene/Camera.hpp"
#include "scene/SceneNode.hpp"
#include "scene/Scene.hpp"
#include "scene/Assets.hpp"
#include "scene/BVH.hpp"

#ifdef MANGO_VULKAN
#include "vk/InstanceVK.hpp"
#endif

#endif //MANGO_MANGO_HPP
