//
// Created by kurono267 on 12.10.2021.
//

#pragma once

#include <string>
#include <vector>

namespace mango {

std::vector<char> readFileBinary(const std::string& filename);
std::string readFileText(const std::string& filename);

}
