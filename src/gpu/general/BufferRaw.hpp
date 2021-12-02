//
//  BufferRaw.hpp
//  MangoRenderer
//
//  Created by kurono267 on 08.05.2021.
//

#pragma once

#include <stdexcept>
#include <string>

namespace mango {

class BufferRaw {
    public:
        BufferRaw(size_t sizeInBytes);
        BufferRaw(size_t sizeInBytes,uint8_t* bytes, bool isOwned = false);
        ~BufferRaw();

        static std::shared_ptr<BufferRaw> make(size_t sizeInBytes){
			return std::make_shared<BufferRaw>(sizeInBytes);
        }

        static std::shared_ptr<BufferRaw> make(size_t sizeInBytes,uint8_t* bytes, bool isOwned = false){
        	return std::make_shared<BufferRaw>(sizeInBytes,bytes,isOwned);
        }
    
        template<typename T>
        const T& get(size_t index) const {
            if(index*sizeof(T) >= _size){
                throw std::runtime_error("BufferRaw::get() Error out of bound "+std::to_string(index)+" with buffer size "+std::to_string(_size));
            }
            return ((T*)_data)[index];
        }
    
        template<typename T>
        T& get(size_t index) {
            if(index*sizeof(T) >= _size){
                throw std::runtime_error("BufferRaw::get() Error out of bound "+std::to_string(index)+" with buffer size "+std::to_string(_size));
            }
            return ((T*)_data)[index];
        }
    
        template<typename T>
        void set(size_t index, const T& value){
            if(index*sizeof(T) >= _size){
                throw std::runtime_error("BufferRaw::set() Error out of bound "+std::to_string(index)+" with buffer size "+std::to_string(_size));
            }
            ((T*)_data)[index] = value;
        }

		uint8_t* getData(){
        	return _data;
        }

        size_t getSize(){
        	return _size;
        }
    protected:
        bool _isOwned;
        size_t _size;
        uint8_t* _data;
};

typedef std::shared_ptr<BufferRaw> spBufferRaw;

}
