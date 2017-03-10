#pragma once
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include <cstdlib>
#include <fstream>
#include <vector>

#include "stb_image.h"

#include <vulkan/vulkan.h>
#include "tinyvk.h"

#include "VECamera.h"
#include "VETexturedPlane.h"
#include "VESceneManager.h"

#pragma comment(lib, "glfw3.lib")

namespace VE
{
	//classes
	class VEngine
	{
	public:
		VEngine() {}

		~VEngine() {}

		void InitializeEngine(GLFWwindow* window);

		static VEngine* getEngine()
		{
			static VEngine* engine;
			if(engine == nullptr)
			{
				engine = new VEngine();
			}
			return engine;
		}

		//void setRenderer(tr_renderer* renderer) { e_renderer = renderer; }

		Camera* getCamera() { return camera; }
		SceneManager* getSceneManager() { return smngr; }
		tr_renderer* getRenderer() { return m_renderer; }
		tr_cmd_pool* getCmdPool() { return m_cmd_pool; }
		tr_cmd** getCmds() { return m_cmds; }
		uint64_t getFrameCount() { return s_frame_count; }

	private:
		Camera* camera = nullptr;
		tr_renderer* m_renderer = nullptr;
		tr_cmd_pool*	m_cmd_pool = nullptr;
		tr_cmd**		m_cmds = nullptr;
		uint64_t     s_frame_count = 0;
		static VEngine* engine;

		SceneManager* smngr = nullptr;
	};

	//structs
	struct Vertex
	{
		glm::vec4 pos;
		glm::vec2 texCoord;

		static VkVertexInputBindingDescription getBindingDescription()
		{
			VkVertexInputBindingDescription bindingDescription = {};
			bindingDescription.binding = 0;
			bindingDescription.stride = sizeof(Vertex);
			bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

			return bindingDescription;
		}

		static tr_vertex_layout getAttributeDescriptions()
		{
			tr_vertex_layout vertex_layout = {};
			vertex_layout.attrib_count = 2;
			vertex_layout.attribs[0].semantic = tr_semantic_position;
			vertex_layout.attribs[0].format = tr_format_r32g32b32a32_float;
			vertex_layout.attribs[0].binding = 0;
			vertex_layout.attribs[0].location = 0;
			vertex_layout.attribs[0].offset = 0;
			vertex_layout.attribs[1].semantic = tr_semantic_texcoord0;
			vertex_layout.attribs[1].format = tr_format_r32g32_float;
			vertex_layout.attribs[1].binding = 0;
			vertex_layout.attribs[1].location = 1;
			vertex_layout.attribs[1].offset = tr_util_format_stride(tr_format_r32g32b32a32_float);

			return vertex_layout;
		}

		bool operator==(const Vertex& other) const
		{
			return pos == other.pos && texCoord == other.texCoord;
		}
	};

	struct uniformBufferObject
	{
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 proj;
	};
}

namespace std
{
	template<> struct hash<VE::Vertex>
	{
		size_t operator()(VE::Vertex const& vertex) const
		{
			return ((hash<glm::vec4>()(vertex.pos) ^
				//(hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^
					(hash<glm::vec2>()(vertex.texCoord) << 1)));
		}
	};
}
