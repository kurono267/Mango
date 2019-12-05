//
// Created by kurono267 on 2019-12-05.
//

#ifndef MANGO_IMAGEBASEDLIGHT_HPP
#define MANGO_IMAGEBASEDLIGHT_HPP

#include <api/Texture.hpp>
#include <unified/Image.hpp>

namespace mango {

class ImageBasedLight {
	public:
		ImageBasedLight();
		ImageBasedLight(const spTexture &image, const int cubemapSize);
		~ImageBasedLight() = default;

		void setImage(const spTexture &image);

		spTexture getImage();

		spTexture getCubeMap();
		spTextureView getCubeMapView();
	protected:
		void convert2cubemap();
		void init();
	protected:
		spTexture _image;
		int _cubemapSize;

		Uniform _cubeMatrices;

		spTexture _cubeTexture;
		spTextureView _cubeTextureView;
		spMesh _cubeMesh;

		spRenderPass _cubeRenderPass;
		spFramebuffer _cubeFrameBuffers[6];

		spCommandBuffer _cubeMapCommands;
		spPipeline _cubeMapPipeline;
		spDescSet _cubeMapDescSet;
};

typedef std::shared_ptr<ImageBasedLight> spImageBasedLight;

}


#endif //MANGO_IMAGEBASEDLIGHT_HPP
