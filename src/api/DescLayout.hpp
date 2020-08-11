//
// Created by kurono267 on 2020-08-10.
//

#ifndef MANGOWORLD_DESCLAYOUT_HPP
#define MANGOWORLD_DESCLAYOUT_HPP

#include "Types.hpp"
#include <vector>

namespace mango {

enum class DescType {
	Uniform = 0x01,
	Storage = 0x02,
	Texture = 0x11,
	StorageTexture = 0x12
};

class DescLayout {
	public:
        struct Entry {
            DescType type;
            size_t binding;
            ShaderStage stage;
        };
    public:
		DescLayout() = default;
		virtual ~DescLayout() = default;

		void set(const DescType& type, size_t binding, const ShaderStage& stage);

		const std::vector<Entry>& getEntries() const;

		virtual void create() = 0;
	protected:
		std::vector<Entry> _entries;
};

typedef std::shared_ptr<DescLayout> spDescLayout;

}

#endif //MANGOWORLD_DESCLAYOUT_HPP
