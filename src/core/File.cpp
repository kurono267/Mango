//
// Created by kurono267 on 12.10.2021.
//

#include "File.hpp"
#include <fstream>

namespace mango {

std::vector<char> readFileBinary(const std::string &filename) {
	std::ifstream file(filename, std::ios::ate | std::ios::binary);

	if (!file.is_open()) {
		throw std::runtime_error("failed to open " + filename);
	}

	size_t fileSize = (size_t) file.tellg();
	std::vector<char> buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);

	file.close();

	return buffer;
}

std::string readFileText(const std::string &filename) {
	std::ifstream file;
	file.open(filename.c_str());
	if (!file.is_open()) {
		throw std::runtime_error("Failed to open " + filename);
	}
	std::string text;
	std::string line;
	while (file.good()) {
		getline(file, line);
		text += line;
		text += "\n";
	}
	text[text.size() - 1] = ' ';
	file.close();
	return text;
}

}
