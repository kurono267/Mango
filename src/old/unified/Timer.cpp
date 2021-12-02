//
// Created by kurono267 on 2020-08-10.
//

#include "Timer.hpp"

using namespace mango;
using namespace std::chrono;

Timer::Timer(float avgCount) : _avgCount(avgCount) {

}

void Timer::start() {
	_prev = steady_clock::now();
}

void Timer::end() {
	auto curr = steady_clock::now();
	float time = duration_cast<duration<float>>(curr-_prev).count();
	if(_avgCount == 1.f){
		_time = time;
	} else {
		_time = (time+_time*_avgCount)/(_avgCount+1);
		if(_count < _avgCount)_count++;
	}
}

float Timer::time() const {
	return _time;
}
