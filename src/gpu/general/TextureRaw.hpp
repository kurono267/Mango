//
//  TextureRaw.hpp
//  MangoRenderer
//
//  Created by kurono267 on 08.05.2021.
//

#pragma once

#include "BufferRaw.hpp"
#include "TextureTypes.hpp"
#include "Texture.hpp"

namespace mango {

class TextureRaw : public BufferRaw {
    public:
        TextureRaw(TextureType type, uint32_t width, uint32_t height, Format format) : BufferRaw(width*height*pixelSize(format)), _layout(type,width,height,1,format) {}
        TextureRaw(TextureType type, uint32_t width, uint32_t height, Format format, uint8_t* bytes, bool isOwned = false) : BufferRaw(width*height*pixelSize(format),bytes, isOwned), _layout(type,width,height,1,format) {}
        TextureRaw(const TextureDesc& layout) : BufferRaw(layout.width * layout.height * layout.depth * pixelSize(layout.format)), _layout(layout) {}

        static std::shared_ptr<TextureRaw> make(TextureType type, uint32_t width, uint32_t height, Format format){
        	return std::make_shared<TextureRaw>(type,width,height,format);
        }

		static std::shared_ptr<TextureRaw> make(TextureType type, uint32_t width, uint32_t height, Format format, uint8_t* bytes, bool isOwned = false){
			return std::make_shared<TextureRaw>(type,width,height,format,bytes,isOwned);
		}

		static std::shared_ptr<TextureRaw> make(const TextureDesc& layout){
			return std::make_shared<TextureRaw>(layout);
		}
        
        template<typename T>
        const T& get(uint32_t x, uint32_t y, uint32_t z = 0) const {
            size_t dataIndex = z*_layout.height*_layout.width+y*_layout.width+x;
            if(dataIndex*pixelSize(_layout.format) >= _size){
                throw std::runtime_error("TextureRaw::get() Error out of bound "+std::to_string(dataIndex)+" with buffer size "+std::to_string(_size));
            }
            return ((T*)_data)[dataIndex];
        }
    
        template<typename T>
        T& get(uint32_t x, uint32_t y, uint32_t z = 0) {
            size_t dataIndex = z*_layout.height*_layout.width+y*_layout.width+x;
            if(dataIndex*pixelSize(_layout.format) >= _size){
                throw std::runtime_error("TextureRaw::get() Error out of bound "+std::to_string(dataIndex)+" with buffer size "+std::to_string(_size));
            }
            return ((T*)_data)[dataIndex];
        }
        
        const TextureDesc& layout() const;
    protected:
        TextureDesc _layout;
};

typedef std::shared_ptr<TextureRaw> spTextureRaw;

}
