//
// Created by kurono267 on 02.04.18.
//

#ifndef UTILS_IMAGE_HPP
#define UTILS_IMAGE_HPP

#include <glm/glm.hpp>
#include <vector>

#include <functional>
#include <memory>

#include <string>

template<typename T>
class ImageBase {
	public:
		ImageBase(){}
		ImageBase(const std::vector<T>& data,const glm::ivec2& size) : _size(size), _data(data){}
		ImageBase(const glm::ivec2& size,const T fill = T()) : _size(size),_data(size.x*size.y){
			for(auto& v : _data)v = fill;
		}
		ImageBase(const ImageBase& image){
			_size = image._size;
			_data = image._data;
		}
		virtual ~ImageBase() = default;

		T& operator()(const int x,const int y) {
			return get(x,y);
		}
		const T& operator()(const int x,const int y) const {
			return get(x,y);
		}

		T& operator()(const glm::vec2& uv)  {
			glm::ivec2 co(uv.x*(float)(_size.x-1),uv.y*(float)(_size.y-1));
			return get(co.x,co.y);
		}
		const T& operator()(const glm::vec2& uv) const {
			glm::ivec2 co(uv.x*(float)(_size.x-1),uv.y*(float)(_size.y-1));
			return get(co.x,co.y);
		}

		const T& get(const int x,const int y) const {
			int cx = std::max(std::min(x,_size.x-1),0);
			int cy = std::max(std::min(y,_size.y-1),0);
			return _data[cy*_size.x+cx];
		}
		T& get(const int x,const int y) {
			int cx = std::max(std::min(x,_size.x-1),0);
			int cy = std::max(std::min(y,_size.y-1),0);
			return _data[cy*_size.x+cx];
		}

		const T& get(const int id) const {
			return _data[id];
		}

		T& get(const int id) {
			return _data[id];
		}

		inline glm::vec2 fsize() const {return glm::vec2(_size.x,_size.y);}

		glm::ivec2 size() const {
			return _size;
		}

		int width() const {
			return _size.x;
		}
		int height() const {
			return _size.y;
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

		operator bool(){
			return !_data.empty();
		}

		typedef std::shared_ptr<ImageBase<T>> Ptr;
		static Ptr make() { return std::make_shared<ImageBase<T>>();}
		static Ptr make(const ImageBase<T>& i) { return std::make_shared<ImageBase<T>>(i); }
	protected:
		std::vector<T> _data;
		glm::ivec2             _size;
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

spImage4b loadImage(const std::string& filename);

#endif //SCANPIPE_ImageBase_HPP
