#define MANGO_VULKAN
#include <mango.hpp>
#include <chrono>

using namespace mango;

class TestApp : public BaseApp {
	public:
		explicit TestApp(const spMainApp& app) : BaseApp(app) {}
		~TestApp() final = default;

		bool init() final {
			auto mainWnd = mainApp.lock();

			auto device = Instance::device();
			std::cout << device->deviceName() << std::endl;

			glm::vec4 colorValue(0.5f,0.5f,1.0f,1.0f);
			_color.create(device,sizeof(glm::vec4),&colorValue);

            _texture = checkboardTexture(1280,720,100);
            auto texView = _texture->createTextureView();

            _computeTexture = device->createTexture(1280,720,1,Format::R16G16B16A16Unorm,TextureType::Input | TextureType::Storage);
            auto computeTexView = _computeTexture->createTextureView();

			_descSet = device->createDescSet();
			_descSet->setUniformBuffer(_color,0,ShaderStage::Fragment);
			_descSet->setTexture(computeTexView,Sampler(),1,ShaderStage::Fragment);
            _descSet->create();

            _computeSet = device->createDescSet();
			_computeSet->setStorageTexture(texView,Sampler(),0,ShaderStage::Compute);
            _computeSet->setStorageTexture(computeTexView,Sampler(),1,ShaderStage::Compute);
            _computeSet->create();

            _compute = device->createCompute("../glsl/compute.glsl",{_computeSet});

            auto screenRTs = device->getScreenRenderTargets();

			PipelineInfo rp;
			rp.viewport(Viewport(glm::vec2(0),mainWnd->frameSize()));
			rp.scissor(glm::ivec2(0),mainWnd->frameSize());
			rp.addShader(ShaderStage::Vertex,"../glsl/test.vert");
			rp.addShader(ShaderStage::Fragment,"../glsl/test.frag");
			rp.setDescSet(_descSet);
			rp.setRenderPass(screenRTs[0]->renderPass());

			_main = device->createPipeline(rp);

			_quad = createQuad();

			for(const auto& screen : screenRTs){
				std::cout << screen->framebuffer()->info() << std::endl;
			}
			_cmdScreen.resize(screenRTs.size());
			for(int i = 0;i<_cmdScreen.size();++i){
				_cmdScreen[i] = device->createCommandBuffer();
				_cmdScreen[i]->begin();

				_cmdScreen[i]->setClearColor(0,glm::vec4(0.0f,1.0f,0.0f,1.0f));
				_cmdScreen[i]->setClearDepthStencil(1,1.0f,0.0f);

				_cmdScreen[i]->beginRenderPass(rp.getRenderPass(),screenRTs[i]->framebuffer(),RenderArea(screenRTs[i]->framebuffer()->getSize(),glm::ivec2(0)));
				_cmdScreen[i]->bindPipeline(_main);
				_cmdScreen[i]->bindDescriptorSet(_main,_descSet);
				_quad->draw(_cmdScreen[i]);
				_cmdScreen[i]->endRenderPass();

				_cmdScreen[i]->end();
			}

			_computeCommandBuffer = device->createCommandBuffer();
			_computeCommandBuffer->begin();
			_computeCommandBuffer->bindCompute(_compute);
			_computeCommandBuffer->bindDescriptorSet(_compute,_computeSet);
			_computeCommandBuffer->dispatch(_computeTexture->width()/16,_computeTexture->height()/16,1);
			_computeCommandBuffer->end();

			_screenAvailable = device->createSemaphore();
			_renderFinish = device->createSemaphore();
			_computeSemaphore = device->createSemaphore();

			return true;
		}
		bool draw() final {
			auto device = Instance::device();
			auto imageIndex = device->nextScreen(_screenAvailable);

			device->submit(_computeCommandBuffer,_screenAvailable,_computeSemaphore);
			device->submit(_cmdScreen[imageIndex],_computeSemaphore,_renderFinish);
			device->present(imageIndex,_renderFinish);

			return true;
		}
		bool update() final {
			return true;
		}

		bool onExit() final {
			Instance::device()->waitIdle();
			return true;
		}
	protected:
		spPipeline _main;
		std::vector<spCommandBuffer> _cmdScreen;
		spMesh _quad;
		spTexture _texture;
		spTexture _computeTexture;

		spCompute _compute;
		spCommandBuffer _computeCommandBuffer;

		Uniform _color;
		spDescSet _descSet;
		spDescSet _computeSet;

		spSemaphore _screenAvailable;
		spSemaphore _computeSemaphore;
		spSemaphore _renderFinish;
};

#include <app/MainAppGLFW.hpp>

int main(){
	auto main = MainAppGLFW::instance();

	main->create("Compute",1280,720);
	main->setBaseApp(std::make_shared<TestApp>(main));

	main->run();

	return 0;
}