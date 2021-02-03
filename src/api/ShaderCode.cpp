//
// Created by kurono267 on 03.02.2021.
//

#include "ShaderCode.hpp"
#include "Utils.hpp"
#include <iostream>

namespace mango {

const std::string ShaderMutableBegin = "#MutableBegin";
const std::string ShaderMutableEnd = "#MutableEnd";

ShaderCode::ShaderCode(const std::string &filename, const ShaderStage &stage) : _stage(stage) {
	_rawCode = readFile(filename);
	preprocess();
}

std::string ShaderCode::getRawCode() {
	return _rawCode;
}

std::string ShaderCode::getCode() {
	return _code;
}

ShaderStage ShaderCode::getStage() {
	return _stage;
}

std::string ShaderCode::getMutable() {
	return _mutable;
}

void ShaderCode::setMutable(const std::string &code) {
	_mutable = code;
	update();
}

// Begin mutable part of shader code tagged by #MutableBegin
// End mutable part of shader code tagged by #MutableEnd
void ShaderCode::preprocess() {
	std::vector<MutableCode> mutableCodes;
	size_t pos = 0;
	size_t prevPos = 0;
	size_t lineID = 0;
	bool isBegin = 0;
	while(pos != std::string::npos){
		pos = _rawCode.find_first_of('\n',pos+1);
		std::string line = _rawCode.substr(prevPos,pos-prevPos);
		size_t beginPos = line.find(ShaderMutableBegin);
		if(beginPos != std::string::npos){
			if(isBegin){
				std::cerr << "Line " << lineID << " Mutable Code doesn't support nested mutable code" << std::endl;
				return;
			}
			isBegin = true;
			MutableCode code;
			code.begin.begin = prevPos;
			code.begin.end = pos+1;
			mutableCodes.push_back(code);
		}
		size_t endPos = line.find(ShaderMutableEnd);
		if(endPos != std::string::npos){
			if(isBegin){
				mutableCodes.back().end.begin = prevPos;
				mutableCodes.back().end.end = pos+1;
				isBegin = false;
			} else {
				std::cerr << "Line " << lineID << " Mutable End without begin" << std::endl;
				return;
			}
		}
		prevPos = pos+1;
		std::cout << "Line " << lineID << " " << line << std::endl;
		lineID++;
	}

	if(mutableCodes.empty()){
		std::cout << "Mutable code doesn't found" << std::endl;
		return;
	}

	_mutablePlace = mutableCodes[0];
	_mutable = _rawCode.substr(_mutablePlace.begin.end,_mutablePlace.end.begin-_mutablePlace.begin.end);

	std::cout << "Mutable Place begin begin " << _mutablePlace.begin.begin << std::endl;
	std::cout << "Mutable Place begin end " << _mutablePlace.begin.end << std::endl;
	std::cout << "Mutable Place end begin " << _mutablePlace.end.begin << std::endl;
	std::cout << "Mutable Place end end " << _mutablePlace.end.end << std::endl;
	update();
}

void ShaderCode::update() {
	if(!_mutablePlace.begin || !_mutablePlace.end)return;

	_code = _rawCode;
	_code = _code.replace(_mutablePlace.begin.begin,_mutablePlace.end.end-_mutablePlace.begin.begin,_mutable);
}

StringRange::operator bool() const {
	if(begin == std::string::npos || end == std::string::npos)return false;
	return true;
}

}
