//
// Created by kurono267 on 02.04.18.
//

#ifndef UTILS_IMAGE_HPP
#define UTILS_IMAGE_HPP

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <vector>

#include <functional>
#include <memory>

#include <string>
#include <iostream>

struct ImageLOD {
    size_t offset;
    glm::ivec2 size;
};

size_t computeImageSizeWithMips(const glm::ivec2& size, size_t mipLevels, std::vector<ImageLOD>& lodOffsets);
size_t computeMaxMipLevels(const glm::ivec2& size);

template<typename T>
class ImageBase {
	public:
		ImageBase(){}
		ImageBase(const std::vector<T>& data,const glm::ivec2& size) : _size(size), _data(data){
		    _lods.resize(1);
		    _lods[0].offset = 0;
		    _lods[0].size = size;
		}
		ImageBase(const glm::ivec2& size,uint32_t mipLevels = 1,const T fill = T()) : _size(size) {
		    size_t dataSize = computeImageSizeWithMips(size,mipLevels,_lods);
		    _data.resize(dataSize);
			for(auto& v : _data)v = fill;
		}
		ImageBase(const ImageBase& image){
			_size = image._size;
			_data = image._data;
			_lods = image._lods;
		}
		virtual ~ImageBase() = default;

		T& operator()(const int x,const int y, int lod = 0) {
			return get(x,y,lod);
		}
		const T& operator()(const int x,const int y, int lod = 0) const {
			return get(x,y,lod);
		}

		T& operator()(const glm::vec2& uv, int lod = 0)  {
			const auto& currLod = _lods[lod];
			glm::ivec2 co(uv.x*(float)(currLod.size.x-1),uv.y*(float)(currLod.size.y-1));
			return get(co.x,co.y,lod);
		}
		const T& operator()(const glm::vec2& uv, int lod = 0) const {
			const auto& currLod = _lods[lod];
			glm::ivec2 co(uv.x*(float)(currLod.size.x-1),uv.y*(float)(currLod.size.y-1));
			return get(co.x,co.y,lod);
		}

		const T& get(const int x,const int y, int lod = 0) const {
		    const auto& currLod = _lods[lod];

			int cx = std::max(std::min(x,currLod.size.x-1),0);
			int cy = std::max(std::min(y,currLod.size.y-1),0);
			return _data[currLod.offset+(cy*currLod.size.x+cx)];
		}
		T& get(const int x,const int y, int lod = 0) {
            const auto& currLod = _lods[lod];

            int cx = std::max(std::min(x,currLod.size.x-1),0);
            int cy = std::max(std::min(y,currLod.size.y-1),0);
            return _data[currLod.offset+(cy*currLod.size.x+cx)];
		}

		const T& get(const int id) const {
			return _data[id];
		}

		T& get(const int id) {
			return _data[id];
		}

		inline glm::vec2 fsize(size_t lod = 0) const {return glm::vec2(_lods[lod].size.x,_lods[lod].size.y);}

		glm::ivec2 size(size_t lod = 0) const {
			return _lods[lod].size;
		}

		int width(size_t lod = 0) const {
			return _lods[lod].size.x;
		}
		int height(size_t lod = 0) const {
            return _lods[lod].size.y;
		}

		const std::vector<T>& data() const { return _data; }

		static ImageBase resize(const ImageBase& image,const glm::ivec2& newSize){
			const int radius = (int)(std::min((float)image.width()/(float)newSize.x,(float)image.height()/(float)newSize.y)*0.5f+1.f);

			ImageBase result(newSize);
			for(int y = 0;y<result.height();++y){
				for(int x = 0;x<result.width();++x){
					glm::vec2 uv((float)x,(float)y);
					uv /= result.fsize()-1.f;
					glm::vec2 baseCoord = uv*(image.fsize()-1.f);

					float n = 0.0f;
					glm::vec3 c(0.0f);
					for(int sy = -radius;sy<=radius;++sy){
						int ny = (int)baseCoord.y+sy;
						if(ny >= image.height() || ny < 0)continue;
						for(int sx = -radius;sx<=radius;++sx){
							int nx = (int)baseCoord.x+sx;
							if(nx >= image.width() || nx < 0)continue;
							float g = 1.0f;
							n += g;
							c += image(nx,ny)*g;
						}
					}
					if(n != 0.0f)c /= n;
					result(x,y) = c;
				}
			}
			return result;
		}

		typename std::vector<T>::iterator begin(){
			return _data.begin();
		}

		const typename std::vector<T>::iterator begin() const {
			return _data.begin();
		}

		typename std::vector<T>::iterator end(){
			return _data.end();
		}

		const typename std::vector<T>::iterator end() const {
			return _data.end();
		}

		void set(const ImageBase& img){
			_data = img._data;
			_size = img._size;
		}

		size_t mipLevels() const {
		    return _lods.size();
		}

		operator bool(){
			return !_data.empty();
		}

		typedef std::shared_ptr<ImageBase<T>> Ptr;
		static Ptr make() { return std::make_shared<ImageBase<T>>();}
		static Ptr make(const ImageBase<T>& i) { return std::make_shared<ImageBase<T>>(i); }

		friend void generateMipMaps(ImageBase<T>& inout){
            std::cout << "generateMipMaps()" << std::endl;
            std::cout << "image size " << inout.width() << " , " << inout.height();

            auto maxLods = computeMaxMipLevels(inout.size());
            std::cout << "Max Lods " << maxLods << std::endl;
            inout._data.resize(computeImageSizeWithMips(inout.size(),maxLods,inout._lods));
            for(auto lod : inout._lods){
                std::cout << "Lod offset " << lod.offset << " size " << glm::to_string(lod.size) << std::endl;
            }
            for(size_t i = 1;i<maxLods;++i){
                for(int y = 0;y<inout.height(i);++y){
                    for(int x = 0;x<inout.width(i);++x){
                        glm::vec2 uv((float)x,(float)y);
                        uv /= inout.fsize(i)-1.f;
                        glm::vec2 baseCoord = uv*(inout.fsize(i-1)-1.f);

                        float n = 0.0f;
                        glm::vec4 c(0.0f);
                        for(int sy = -1;sy<=1;++sy){
                            int ny = (int)baseCoord.y+sy;
                            if(ny >= inout.height(i-1) || ny < 0)continue;
                            for(int sx = -1;sx<=1;++sx){
                                int nx = (int)baseCoord.x+sx;
                                if(nx >= inout.width(i-1) || nx < 0)continue;
                                float g = 1.0f;
                                n += g;
                                c += glm::vec4(inout(nx,ny,i-1))*g;
                            }
                        }
                        if(n != 0.0f)c /= n;
                        inout(x,y,i) = c;
                    }
                }
            }
        }
	protected:
		std::vector<T> _data;
		glm::ivec2             _size;
        std::vector<ImageLOD> _lods;
};

template<typename T>
std::pair<T,T> imageBound(const ImageBase<T>& image){
	std::pair<T,T> bound(T(std::numeric_limits<float>::infinity()),T(-std::numeric_limits<float>::infinity()));
	for(int y = 0;y<image.height();++y){
		for(int x = 0;x<image.width();++x){
			bound.first = std::min(bound.first,image(x,y));
			bound.second = std::max(bound.second,image(x,y));
		}
	}
	return bound;
}

typedef ImageBase<glm::vec3> Image;
typedef ImageBase<glm::vec4> Image4f;
typedef ImageBase<float> Image1f;
typedef ImageBase<glm::u8vec3> Image3b;
typedef ImageBase<glm::u8vec4> Image4b;

typedef std::shared_ptr<Image> spImage;
typedef std::shared_ptr<Image4f> spImage4f;
typedef std::shared_ptr<Image1f> spImage1f;
typedef std::shared_ptr<Image3b> spImage3b;
typedef std::shared_ptr<Image4b> spImage4b;

template<typename T>
ImageBase<T> downsampling(const ImageBase<T>& in,std::function<bool(const T&)> check){
	int nWidth = in.width()/2;
	int nHeight = in.height()/2;
	ImageBase<T> data(glm::ivec2(nWidth,nHeight),T());
	for(int y = 0;y<nHeight;++y){
		for(int x = 0;x<nWidth;++x){
			T v = T(0.0f); float n = 0.f;
			for(int y2 = -1;y2<2;++y2){
				for(int x2 = -1;x2<2;++x2){
					int oX = (x*2+x2); int oY = (y*2+y2);
					if(oX >= in.width() || oY >= in.height() || oX < 0 || oY < 0)continue;
					T d = in(oX,oY);
					if(check(d))continue;
					v += d;
					n += 1.0f;
				}
			}
			if(n == 0.0f)v = T(1.0f);
			else if(n != 1.0f)v /= n;
			data(x,y) = v;
		}
	}
	return data;
}

template<typename T>
ImageBase<T> crop(const ImageBase<T>& in,const glm::ivec2& rect_min,const glm::ivec2& rect_max){
	glm::ivec2 newSize = rect_max-rect_min;
	ImageBase<T> result(newSize);
	for(int x = 0;x<newSize.x;++x){
		for(int y = 0;y<newSize.y;++y){
			auto v = in(x+rect_min.x,y+rect_min.y);
			result(x,y) = v;
		}
	}
	return result;
}

// Check point at line or not
inline bool isLinePoint(const glm::ivec2& start,const glm::ivec2& end,const int x,const int y){
	float dist = std::abs((float)(end.y-start.y)*x-(float)(end.x-start.x)*y+(float)end.x*start.y-(float)end.y*start.x);
	dist /= glm::length(glm::vec2(end)-glm::vec2(start));
	return dist < 0.5f;
}

template<typename T>
glm::vec4 filterLinear(const ImageBase<T>& image, const glm::vec2& uv, size_t lod){
    glm::vec2 coord(uv*(image.fsize(lod)-1.f));
    glm::ivec2 startCoord = glm::floor(coord);
    glm::ivec2 endCoord = startCoord+1;
    endCoord.x = std::min(endCoord.x,image.size(lod).x-1);
    endCoord.y = std::min(endCoord.y,image.size(lod).y-1);
    glm::vec2 delta(glm::fract(coord));

    glm::vec4 p00 = image(startCoord.x,startCoord.y,lod);
    glm::vec4 p01 = image(startCoord.x,endCoord.y,lod);
    glm::vec4 p10 = image(endCoord.x,startCoord.y,lod);
    glm::vec4 p11 = image(endCoord.x,endCoord.y,lod);

    auto cX0 = glm::mix(p00,p10,delta.x);
    auto cX1 = glm::mix(p01,p11,delta.x);

    return glm::mix(cX0,cX1,delta.y);
}

template<typename T>
glm::vec4 filterTrilinear(const ImageBase<T>& image,const glm::vec2& uv, const glm::vec2& dUVx, const glm::vec2& dUVy){
	float deltaMaxSqr = std::max(glm::dot(dUVx, dUVx), glm::dot(dUVy, dUVy));
	float lod = 0.5f * log2(deltaMaxSqr);

	int currLod = std::min(std::max((int)std::floor(lod),0),(int)image.mipLevels()-1);
	int nextLod = std::min(currLod+1,(int)image.mipLevels()-1);

	if(currLod == nextLod)return filterLinear(image,uv,currLod);

	auto currLodColor = filterLinear(image,uv,currLod);
	auto nextLodColor = filterLinear(image,uv,nextLod);

	return glm::mix(currLodColor,nextLodColor,glm::fract(lod));
}

spImage4b loadImage(const std::string& filename);
spImage4f loadImageHDRI(const std::string& filename);

#endif //SCANPIPE_ImageBase_HPP
