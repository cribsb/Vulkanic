#define TINY_RENDERER_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "tinyvk.h"
#include <cstdint>
#include <cstdio>

//#include <vld.h>

#include "Engine/Helper.hpp"
#include "Engine/VECamera.h"
#include "Engine/VETexturedPlane.h"
#include "Engine/VE3dmodel.h"
#include "Engine/VESceneManager.h"
#include <sstream>
#include <iostream>
#include "Engine/VEGame.h"
#include "Engine/Example/example.h"

const uint32_t      kImageCount = 3;
tr_renderer*        l_renderer  = nullptr;
tr_cmd_pool*        l_cmd_pool  = nullptr;
tr_cmd**            l_cmds      = nullptr;
GLFWwindow*         window      = nullptr;
uint32_t            s_window_width;
uint32_t            s_window_height; 

VE::Camera*         camera = nullptr;
VE::TexturedPlane*  plane  = nullptr;
VE::DModel*         model  = nullptr;
VE::Game*           game   = nullptr;

using namespace VE;

static void app_glfw_error(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

void renderer_log(tr_log_type type, const char* msg, const char* component)
{
	switch(type)
	{
		//case tr_log_type_info  : {CI_LOG_I("[" << component << "] : " << msg);} break;
		//case tr_log_type_warn  : {CI_LOG_W("[" << component << "] : " << msg);} break;
		//case tr_log_type_debug : {CI_LOG_E("[" << component << "] : " << msg);} break;
		//case tr_log_type_error : {CI_LOG_D("[" << component << "] : " << msg);} break;
	default: break;
	}
}

VKAPI_ATTR VkBool32 VKAPI_CALL vulkan_debug(
	VkDebugReportFlagsEXT      flags,
	VkDebugReportObjectTypeEXT objectType,
	uint64_t                   object,
	size_t                     location,
	int32_t                    messageCode,
	const char*                pLayerPrefix,
	const char*                pMessage,
	void*                      pUserData
)
{
	if(flags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT)
	{
		//CI_LOG_I( "[" << pLayerPrefix << "] : " << pMessage << " (" << messageCode << ")" );
	}
	else if(flags & VK_DEBUG_REPORT_WARNING_BIT_EXT)
	{
		//CI_LOG_W( "[" << pLayerPrefix << "] : " << pMessage << " (" << messageCode << ")" );
	}
	else if(flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT)
	{
		//CI_LOG_I( "[" << pLayerPrefix << "] : " << pMessage << " (" << messageCode << ")" );
	}
	else if(flags & VK_DEBUG_REPORT_ERROR_BIT_EXT)
	{
		//CI_LOG_E( "[" << pLayerPrefix << "] : " << pMessage << " (" << messageCode << ")" );
	}
	else if(flags & VK_DEBUG_REPORT_DEBUG_BIT_EXT)
	{
		//CI_LOG_D( "[" << pLayerPrefix << "] : " << pMessage << " (" << messageCode << ")" );
	}
	return VK_FALSE;
}

std::vector<uint8_t> load_file(const std::string& path)
{
	std::ifstream is;
	is.open(path.c_str(), std::ios::in | std::ios::binary);
	assert(is.is_open());

	is.seekg(0, std::ios::end);
	std::vector<uint8_t> buffer(is.tellg());
	assert(0 != buffer.size());

	is.seekg(0, std::ios::beg);
	is.read(reinterpret_cast<char*>(buffer.data()), buffer.size());

	return buffer;
}

void VEngine::InitializeEngine(GLFWwindow* window)
{
	std::vector<const char*> instance_layers = {
#if defined(_DEBUG)
		//"VK_LAYER_LUNARG_api_dump",
		"VK_LAYER_LUNARG_core_validation",
		"VK_LAYER_LUNARG_swapchain",
		"VK_LAYER_LUNARG_image",
		"VK_LAYER_LUNARG_parameter_validation"
#endif
	};

	std::vector<const char*> device_layers = {
#if defined(_DEBUG)
		//"VK_LAYER_LUNARG_api_dump",
		"VK_LAYER_LUNARG_core_validation",
		"VK_LAYER_LUNARG_swapchain",
		"VK_LAYER_LUNARG_image",
		"VK_LAYER_LUNARG_parameter_validation"
#endif
	};

	int width = 0;
	int height = 0;
	glfwGetWindowSize(window, &width, &height);
	s_window_width = static_cast<uint32_t>(width);
	s_window_height = static_cast<uint32_t>(height);

	tr_renderer_settings settings = {nullptr};
	settings.handle.hinstance = ::GetModuleHandle(nullptr);
	settings.handle.hwnd = glfwGetWin32Window(window);
	settings.width = s_window_width;
	settings.height = s_window_height;
	settings.swapchain.image_count = kImageCount;
	settings.swapchain.sample_count = tr_sample_count_1;
	settings.swapchain.color_format = tr_format_b8g8r8a8_unorm;
	settings.swapchain.depth_stencil_format = tr_format_d32_float_s8_uint;
	settings.swapchain.depth_stencil_clear_value = {1.0f, 0};
	settings.log_fn = renderer_log;
	settings.vk_debug_fn = vulkan_debug;
	settings.instance_layers.count = static_cast<uint32_t>(instance_layers.size());
	settings.instance_layers.names = instance_layers.empty() ? nullptr : instance_layers.data();
	settings.device_layers.count = static_cast<uint32_t>(device_layers.size());
	settings.device_layers.names = device_layers.data();
	
	tr_create_renderer("Vulkanic", &settings, &m_renderer);

	tr_create_cmd_pool(m_renderer, m_renderer->graphics_queue, false, &m_cmd_pool);
	tr_create_cmd_n(m_cmd_pool, false, kImageCount, &m_cmds);

	l_renderer = m_renderer;
	l_cmd_pool = m_cmd_pool;
	l_cmds = m_cmds;

	camera = new Camera();
	camera->setPosition({0.0f, 0.0f, 3.0f});
	//plane = new TexturedPlane();
	//plane->Create("assets/box_panel.jpg");
	//model = new DModel();
	//model->Create("assets/teapot.obj", 0.005f);

	input = new Input(window);

	scene_manager = new SceneManager();
	//Scene* s = new Scene();
	//s->addObject(plane);
	//s->addObject(model);
	//scene_manager->addScene(s, "startscene");
	//hoeft niet
	//scene_manager->setCurrentScene(s);
}

void VEngine::Quit()
{
	glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void destroy_tiny_renderer()
{
	tr_destroy_renderer(l_renderer);
}

void Update(float dt)
{
	//VEngine::getEngine()->getCamera()->Update();
	VEngine::getEngine()->getSceneManager()->getCurrentScene()->Update(dt);
}

void draw_frame(float dt)
{
	Update(dt);

	uint32_t frameIdx = VEngine::getEngine()->getFrameCount() % l_renderer->settings.swapchain.image_count;

	tr_fence* image_acquired_fence = l_renderer->image_acquired_fences[frameIdx];
	tr_semaphore* image_acquired_semaphore = l_renderer->image_acquired_semaphores[frameIdx];
	tr_semaphore* render_complete_semaphores = l_renderer->render_complete_semaphores[frameIdx];

	tr_acquire_next_image(l_renderer, image_acquired_semaphore, image_acquired_fence);

	uint32_t swapchain_image_index = l_renderer->swapchain_image_index;
	tr_render_target* render_target = l_renderer->swapchain_render_targets[swapchain_image_index];
	render_target->depth_stencil_attachment;

	tr_cmd* cmd = l_cmds[frameIdx];

	tr_begin_cmd(cmd);

	tr_cmd_render_target_transition(cmd, render_target, tr_texture_usage_present, tr_texture_usage_color_attachment);
	
	tr_cmd_set_viewport(cmd, 0, 0, s_window_width, s_window_height, 0.0f, 1.0f);
	tr_cmd_set_scissor(cmd, 0, 0, s_window_width, s_window_height);
	tr_cmd_begin_render(cmd, render_target);
	tr_clear_value clear_value = {0.0f, 0.0f, 0.0f, 0.0f};
	tr_cmd_clear_color_attachment(cmd, 0, &clear_value);

	//plane->Draw(cmd);
	//model->Draw(cmd);
	VEngine::getEngine()->getSceneManager()->getCurrentScene()->Draw(cmd, dt);

	tr_cmd_end_render(cmd);

	tr_cmd_render_target_transition(cmd, render_target, tr_texture_usage_color_attachment, tr_texture_usage_present);

	tr_end_cmd(cmd);

	tr_queue_submit(l_renderer->graphics_queue, 1, &cmd, 1, &image_acquired_semaphore, 1, &render_complete_semaphores);
	tr_queue_present(l_renderer->present_queue, 1, &render_complete_semaphores);

	tr_queue_wait_idle(l_renderer->graphics_queue);
}

VEngine* engine;

double oldMouseX, oldMouseY;

int main(int argc, char **argv)
{
	glfwSetErrorCallback(app_glfw_error);
	if(!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	window = glfwCreateWindow(1280, 720, "VULKANIC", nullptr, nullptr);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	engine = VEngine::getEngine();
	engine->InitializeEngine(window);
	game = new ExampleGame();
	game->Create();
	game->Initialize();

	//glfwSetKeyCallback(window, key_callback);
	auto mouseCallback = [](GLFWwindow* window, double xpos, double ypos)
	{
		engine->getCamera()->Rotate(glm::vec3(0.01f*(xpos-oldMouseX), 0.01f*(ypos-oldMouseY), 0.0f));
		glfwGetCursorPos(window, &oldMouseX, &oldMouseY);
	};

	glfwSetCursorPosCallback(window, mouseCallback);
	glfwGetCursorPos(window, &oldMouseX, &oldMouseY);
	
	// The fps stuff;
	int    frames;
	double fps, currentFrame, lastFrame, deltaTime;
	char   title[150];
	currentFrame = glfwGetTime();
	lastFrame = currentFrame;
	frames = 0;

	while(!glfwWindowShouldClose(window))
	{
		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		if((currentFrame - lastFrame) > 1.0 || frames == 0)
		{
			fps = static_cast<double>(frames) / (deltaTime);
			sprintf_s(title, "VULKANIC ALPHA (%.1f FPS)", fps);
			glfwSetWindowTitle(window, title);
			frames = 0;
		}
		frames++;
		draw_frame(deltaTime);
		glfwPollEvents();
	}

	destroy_tiny_renderer();

	glfwDestroyWindow(window);
	glfwTerminate();
	delete engine;
	delete plane;
	delete model;
	return EXIT_SUCCESS;
}