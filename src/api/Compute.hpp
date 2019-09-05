//
// Created by kurono267 on 2019-09-05.
//

#ifndef MANGO_COMPUTE_HPP
#define MANGO_COMPUTE_HPP

#include <string>
#include "DescSet.hpp"

namespace mango {

class Compute {
	public:
		Compute(const std::string& filename, const std::vector<spDescSet>& descSets);

		virtual spSemaphore run(const spSemaphore& waitForIt,const int sizeX, const int sizeY = 1, const int sizeZ = 1) = 0;
	protected:
		virtual void init() = 0;
	protected:
		std::string _filename;
		std::vector<spDescSet> _descSets;
};

}

#endif //MANGO_COMPUTE_HPP
