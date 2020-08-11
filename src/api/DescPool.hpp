//
// Created by kurono267 on 2020-08-10.
//

#ifndef MANGOWORLD_DESCPOOL_HPP
#define MANGOWORLD_DESCPOOL_HPP

#include "Types.hpp"
#include "DescLayout.hpp"

namespace mango {

class DescSet;
typedef std::shared_ptr<DescSet> spDescSet;

class DescPool {
	public:
		virtual std::vector<spDescSet> create(size_t count) = 0;
		virtual spDescSet create() = 0;

		virtual spDescLayout getLayout() = 0;
	protected:
		DescPool() = default;
};

typedef std::shared_ptr<DescPool> spDescPool;

}

#endif //MANGOWORLD_DESCPOOL_HPP
