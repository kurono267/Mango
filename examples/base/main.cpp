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

			_instance = std::make_unique<vulkan::InstanceVK>();
			_instance->init("Test",mainWnd->window(),mainWnd->wndSize());

			auto device = _instance->device();
			std::cout << device->device_name() << std::endl;

			PipelineInfo rp;
			rp.viewport(Viewport(glm::vec2(0),mainWnd->wndSize()));
			rp.scissor(glm::ivec2(0),mainWnd->wndSize());

			glm::vec4 colorValue(0.5f,0.5f,1.0f,1.0f);
			_color.create(device,sizeof(glm::vec4),&colorValue);

            _texture = checkboardTexture(device,1280,720,100);
            auto texView = _texture->createTextureView();

			_descSet = device->createDescSet();
			_descSet->setUniformBuffer(_color,0,ShaderStage::Fragment);
			_descSet->setTexture(texView,Sampler(),1,ShaderStage::Fragment);
            _descSet->create();

			_main = device->createPipeline(rp);
			_main->addShader(ShaderStage::Vertex,"../glsl/test.vert");
			_main->addShader(ShaderStage::Fragment,"../glsl/test.frag");
			_main->setDescSet(_descSet);

			_quad = createQuad(device);

			spRenderPass renderPass = device->getScreenRenderPass();

			_main->setRenderPass(renderPass);
			_main->create();

			auto screenBuffers = device->getScreenbuffers();
			for(const auto& screen : screenBuffers){
				std::cout << screen->info() << std::endl;
			}
			_cmdScreen.resize(screenBuffers.size());
			for(int i = 0;i<_cmdScreen.size();++i){
				_cmdScreen[i] = device->createCommandBuffer();
				_cmdScreen[i]->begin();

				_cmdScreen[i]->setClearColor(0,glm::vec4(0.0f,1.0f,0.0f,1.0f));
				_cmdScreen[i]->setClearDepthStencil(1,1.0f,0.0f);

				_cmdScreen[i]->beginRenderPass(renderPass,screenBuffers[i],RenderArea(screenBuffers[i]->getSize(),glm::ivec2(0)));
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
			auto device = _instance->device();
			auto imageIndex = device->nextScreen(_screenAvailable);

			device->submit(_cmdScreen[imageIndex],_screenAvailable,_renderFinish);
			device->present(imageIndex,_renderFinish);

			return true;
		}
		bool update() final {
			return true;
		}
		
		bool onKey(const GLFWKey& key) final {
			return true;
		}
		bool onMouse(const GLFWMouse& mouse) final {
			return true;
		}
		bool onExit() final {
			return true;
		}
	protected:
		std::unique_ptr<Instance> _instance;

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
	spMainApp main = MainApp::instance();
	spBaseApp app = std::make_shared<TestApp>(main);

	main->create("Base",1280,720);
	main->setBaseApp(app);

	main->run();

	return 0;
}