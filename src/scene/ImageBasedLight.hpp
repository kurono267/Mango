//
// Created by kurono267 on 2019-12-05.
//

#ifndef MANGO_IMAGEBASEDLIGHT_HPP
#define MANGO_IMAGEBASEDLIGHT_HPP

#include <api/Texture.hpp>
#include <unified/Image.hpp>
#include <unified/Uniform.hpp>
#include <unified/Mesh.hpp>

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

		spTexture getFilter();
		spTextureView getFilterView();

		spTexture getBRDF();
		spTextureView getBRDFView();
	protected:
		void convert2cubemap();
		void filter();
		void brdf();
		void init();
	protected:
		spTexture _image;
		int _cubemapSize;

		Uniform _cubeMatrices;
		spMesh _cubeMesh;
		spMesh _quadMesh;

		spTexture _cubeTexture;
		spTextureView _cubeTextureView;

		spTexture _filterTexture;
		spTextureView _filterTextureView;

		spTexture _brdfTexture;
		spTextureView _brdfTextureView;

		spRenderPass _cubeRenderPass;
		std::vector<spFramebuffer> _cubeFrameBuffers;

		spCommandBuffer _cubeMapCommands;
		spPipeline _cubeMapPipeline;
		spDescSet _cubeMapDescSet;

		spCommandBuffer _filterCommands;
		spPipeline _filterPipeline;
		spDescSet _postDescSet;

		std::vector<spFramebuffer> _filterFrameBuffers;

		spCommandBuffer _brdfCommands;
		spPipeline _brdfPipeline;

		spRenderTarget _brdfRenderTarget;

};

typedef std::shared_ptr<ImageBasedLight> spImageBasedLight;

}


#endif //MANGO_IMAGEBASEDLIGHT_HPP
