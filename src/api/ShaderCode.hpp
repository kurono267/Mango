//
// Created by kurono267 on 03.02.2021.
//

#pragma once

#include "Types.hpp"

namespace mango {

struct StringRange {
	operator bool() const;

	size_t begin = std::string::npos;
	size_t end = std::string::npos;
};

class ShaderCode {
		struct MutableCode {
			StringRange begin;
			StringRange end;
		};
	public:
		ShaderCode(const std::string &filename, const mango::ShaderStage &stage);

		std::string getRawCode();
		std::string getCode();
		ShaderStage getStage();

		std::string getMutable();
		void setMutable(const std::string &code);
	private:
		std::string _rawCode;
		std::string _code;
		ShaderStage _stage;
		std::string _mutable;
		MutableCode _mutablePlace;
	private:
		void preprocess();
		void update();
};

}


