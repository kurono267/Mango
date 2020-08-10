//
// Created by kurono267 on 2020-08-10.
//

#include "Timer.hpp"

using namespace mango;
using namespace std::chrono;

void Timer::start() {
	_prev = system_clock::now();
}

void Timer::end() {
	auto curr = system_clock::now();
	_time = duration_cast<duration<float>>(curr-_prev).count();
}

float Timer::time() {
	return _time;
}
