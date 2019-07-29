//
// Created by kurono267 on 09.07.18.
//

#ifndef MANGO_UTILS_HPP
#define MANGO_UTILS_HPP

#include <vector>
#include <string>
#include <fstream>

#include "Types.hpp"

namespace mango {

std::vector<char> readFileBinary(const std::string& filename);
std::string 	  readFile(const std::string& filename);

std::string to_string(Format value);

inline bool hasStencilComponent(Format format) {
    return format == Format::D32SfloatS8Uint || format == Format::D24UnormS8Uint;
}

};

#endif //MANGO_UTILS_HPP
