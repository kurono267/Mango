#include <mango.hpp>
#include <chrono>
#include <api/instance.hpp>
#include <vk/instance_vk.hpp>
#include <vk/pipeline_vk.hpp>
#include <vk/framebuffer_vk.hpp>

using namespace mango;

class TestApp : public BaseApp {
	public:
		TestApp(spMainApp app) : BaseApp(app) {}
		~TestApp() final {}

		bool init(){
			auto mainWnd = mainApp.lock();

			_instance = std::make_unique<vulkan::InstanceVK>();
			_instance->init("Test",mainWnd->window(),mainWnd->wndSize());

			auto device = _instance->device();
			std::cout << device->device_name() << std::endl;

			RenderPattern rp;

			spPipeline pipeline = device->createPipeline(rp);
			pipeline->addShader(ShaderStage::Vertex,"glsl/test.vert");
			pipeline->addShader(ShaderStage::Fragment,"glsl/test.frag");

			spRenderPass pass = device->createRenderPass();

			pipeline->setRenderPass(pass);
			pipeline->create();

			auto screenBuffers = device->getScreenbuffers(pipeline);
			for(auto screen : screenBuffers){
				std::cout << screen->info() << std::endl;
			}

			return true;
		}
		bool draw(){
			return true;
		}
		bool update(){
			return true;
		}
		
		bool onKey(const GLFWKey& key){
			return true;
		}
		bool onMouse(const GLFWMouse& mouse){
			return true;
		}
		bool onExit(){
			return true;
		}
	protected:
		std::unique_ptr<Instance> _instance;
};

int main(){
	spMainApp main = MainApp::instance();
	spBaseApp app = std::make_shared<TestApp>(main);

	main->create("Base",1280,720);
	main->setBaseApp(app);

	main->run();

	return 0;
}