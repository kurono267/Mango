#define MANGO_VULKAN
#include <mango.hpp>
#include <app/MainAppGLFW.hpp>
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
			_color.create(sizeof(glm::vec4),BufferType::Uniform,&colorValue);

			_texture = checkboardTexture(1280,720,100);
            auto textureView = _texture->createTextureView();

            auto descLayout = device->createDescLayout();
            descLayout->set(DescType::Uniform,0,ShaderStage::Fragment);
            descLayout->set(DescType::Texture,1,ShaderStage::Fragment);
            descLayout->create();

            auto descPool = device->createDescPool(1,descLayout);
            _descSet = descPool->create();

			_descSet->setUniformBuffer(_color,0,ShaderStage::Fragment);
			_descSet->setTexture(textureView,Sampler(),1,ShaderStage::Fragment);
            _descSet->write();

            auto screenRTs = device->getScreenRenderTargets();

			PipelineInfo rp;
			rp.viewport(Viewport(glm::vec2(0),mainWnd->frameSize()));
			rp.scissor(glm::ivec2(0),mainWnd->frameSize());
			rp.addShader(ShaderStage::Vertex,"../glsl/test.vert");
			rp.addShader(ShaderStage::Fragment,"../glsl/test.frag");
			rp.setDescLayout(descLayout);
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

			_screenAvailable = device->createSemaphore();
			_renderFinish = device->createSemaphore();

			return true;
		}
		bool draw() final {
			auto device = Instance::device();
			auto imageIndex = device->nextScreen(_screenAvailable);

			device->submit(_cmdScreen[imageIndex],_screenAvailable,_renderFinish);
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

		Uniform _color;
		spDescSet _descSet;

		spSemaphore _screenAvailable;
		spSemaphore _renderFinish;
};

int main(){
	auto main = MainAppGLFW::instance();

	main->create("Base",1280,720);
	main->setBaseApp(std::make_shared<TestApp>(main));

	main->run();

	return 0;
}