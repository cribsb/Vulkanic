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
#include <sstream>
#include <iostream>

const uint32_t kImageCount = 3;
tr_renderer*        l_renderer = nullptr;
tr_cmd_pool*        l_cmd_pool = nullptr;
tr_cmd**            l_cmds = nullptr;
uint32_t            s_window_width;
uint32_t            s_window_height;

VE::Camera*         camera = nullptr;
VE::TexturedPlane*  plane = nullptr;
VE::DModel*          model = nullptr;

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
		"VK_LAYER_LUNARG_api_dump",
		"VK_LAYER_LUNARG_core_validation",
		"VK_LAYER_LUNARG_swapchain",
		"VK_LAYER_LUNARG_image",
		"VK_LAYER_LUNARG_parameter_validation"
#endif
	};

	std::vector<const char*> device_layers = {
#if defined(_DEBUG)
		"VK_LAYER_LUNARG_api_dump",
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

	m_camera = new Camera();
	m_camera->setPosition({0.0f, 0.0f, 3.0f});
	plane = new TexturedPlane(m_renderer, m_cmds);
	plane->Create("assets/box_panel.jpg");
	model = new DModel(m_renderer);
	model->Create("assets/teapot.obj", 0.005f);

	l_renderer = m_renderer;
	l_cmd_pool = m_cmd_pool;
	l_cmds = m_cmds;
}

void destroy_tiny_renderer()
{
	tr_destroy_renderer(l_renderer);
}

void Update()
{
	//VEngine::getEngine()->getCamera()->Update();
}

void draw_frame()
{
	Update();

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

	plane->Draw(cmd);
	model->Draw(cmd);

	tr_cmd_end_render(cmd);

	tr_cmd_render_target_transition(cmd, render_target, tr_texture_usage_color_attachment, tr_texture_usage_present);

	tr_end_cmd(cmd);

	tr_queue_submit(l_renderer->graphics_queue, 1, &cmd, 1, &image_acquired_semaphore, 1, &render_complete_semaphores);
	tr_queue_present(l_renderer->present_queue, 1, &render_complete_semaphores);

	tr_queue_wait_idle(l_renderer->graphics_queue);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
		
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

	GLFWwindow* window = glfwCreateWindow(1280, 720, "VULKANIC", nullptr, nullptr);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	engine = VEngine::getEngine();
	engine->InitializeEngine(window);

	glfwSetKeyCallback(window, key_callback);
	auto mouseCallback = [](GLFWwindow* window, double xpos, double ypos)
	{
		engine->getCamera()->Rotate(glm::vec3(0.01f*(xpos-oldMouseX), 0.01f*(ypos-oldMouseY), 0.0f));
		glfwGetCursorPos(window, &oldMouseX, &oldMouseY);
	};

	auto mouseButtonCallback = [](GLFWwindow* window, int button, int action, int mods)
	{
		if(button == GLFW_MOUSE_BUTTON_1 && action == action == GLFW_PRESS)
		{
			engine->getCamera()->MoveBy(glm::vec3(0.f, 0.f, 0.1f));
		}
		if(button == GLFW_MOUSE_BUTTON_2 && action == action == GLFW_PRESS)
		{
			engine->getCamera()->MoveBy(glm::vec3(0.f, 0.f, -0.1f));
		}
	};

	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwGetCursorPos(window, &oldMouseX, &oldMouseY);
	
	// The fps stuff;
	int    frames;
	double fps, time, t0;
	char   title[150];
	frames = 0;
	t0 = glfwGetTime();

	while(!glfwWindowShouldClose(window))
	{
		time = glfwGetTime();
		if((time - t0) > 1.0 || frames == 0)
		{
			fps = static_cast<double>(frames) / (time - t0);
			sprintf_s(title, "VULKANIC ALPHA (%.1f FPS)", fps);
			glfwSetWindowTitle(window, title);
			t0 = time;
			frames = 0;
		}
		frames++;
		draw_frame();
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