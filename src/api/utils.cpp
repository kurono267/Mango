//
// Created by kurono267 on 09.07.18.
//

#include <iostream>
#include "utils.hpp"

using namespace mango;

std::vector<char> readFileBinary(const std::string& filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("failed to open file!");
    }

    size_t fileSize = (size_t) file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();

    return buffer;
}

std::string readFile(const std::string& filename){
	std::ifstream file;
	file.open(filename.c_str());
	if(!file.is_open()){
		std::cout << "Open " << filename << " Failed" << std::endl;
		return std::string();
	}
	std::string text;
	std::string line;
	while (file.good()) {
		getline(file, line);
		text += line;
		text += "\n";
	}
	text[text.size()-1] = ' ';
	file.close();
	return text;
}
