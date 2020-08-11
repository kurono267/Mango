//
// Created by kurono267 on 2020-08-10.
//

#include "DescLayout.hpp"

using namespace mango;

void DescLayout::set(const DescType& type, size_t binding, const ShaderStage& stage) {
	Entry entry = {type,binding,stage};
	_entries.emplace_back(entry);
}

const std::vector<DescLayout::Entry> &DescLayout::getEntries() const {
    return _entries;
}

