//
// Created by kurono267 on 08.03.18.
//

#pragma once

#include "Device.hpp"
#include "Buffer.hpp"

namespace mango {

class InstanceAPI {
	public:
		InstanceAPI(){}
		virtual ~InstanceAPI() = default;
		InstanceAPI(const InstanceAPI& i) = delete;

		virtual void init(const std::string& title, void* window,const glm::ivec2& size) = 0;
		virtual spDevice device() = 0;

		virtual void release() = 0;
};

class Instance {
	public:
		template<typename InstanceType>
		static void init(const std::string& title, void* window,const glm::ivec2& size) {
			auto& that = get();
			if(that._impl){
				throw std::runtime_error("Instance init should call once");
			}
			that._impl = std::make_unique<InstanceType>();
			that._impl->init(title,window,size);
		}

		template<typename T = Device>
		static std::shared_ptr<T> device(){
			auto& that = get();
			return std::dynamic_pointer_cast<T>(that._impl->device());
		}

		static void release(){
			auto& that = get();
			that._impl->release();
		}
	private:
		Instance() = default;
		~Instance() = default;
		Instance(const Instance& i) = delete;

		static Instance& get();
		std::unique_ptr<InstanceAPI> _impl;
};

};
