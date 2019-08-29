//
// Created by kurono267 on 2019-08-29.
//

#include "Instance.hpp"

using namespace mango;

Instance& Instance::get() {
	static Instance instance;
	return instance;
}