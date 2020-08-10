//
// Created by kurono267 on 2020-08-10.
//

#ifndef MANGOWORLD_TIMER_HPP
#define MANGOWORLD_TIMER_HPP

#include <chrono>

namespace mango {

class Timer {
	public:
		void start();
		void end();

		float time();

	protected:
		std::chrono::system_clock::time_point _prev;
		float _time;
};

};

#endif //MANGOWORLD_TIMER_HPP
