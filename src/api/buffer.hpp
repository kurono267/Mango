//
// Created by kurono267 on 06.07.18.
//

#ifndef MANGO_BUFFER_HPP
#define MANGO_BUFFER_HPP

#include <cstddef>
#include <stdexcept>
#include <memory>

namespace mango {

class Buffer {
	public:
		Buffer() = default;
		virtual ~Buffer() = default;

		virtual void set(const size_t& size,const void* data) = 0;
};

typedef std::shared_ptr<Buffer> spBuffer;

};

#endif //MANGO_BUFFER_HPP
