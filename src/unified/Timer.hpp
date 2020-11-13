//
// Created by kurono267 on 2020-08-10.
//

#ifndef MANGOWORLD_TIMER_HPP
#define MANGOWORLD_TIMER_HPP

#include <chrono>

namespace mango {

class Timer {
	public:
		explicit Timer(float avgCount = 1.f);

		void start();
		void end();

		float time() const;

	protected:
		std::chrono::steady_clock::time_point _prev;
		float _time = 0.f;
		float _count = 0.f;
		float _avgCount;
};

};

#endif //MANGOWORLD_TIMER_HPP
