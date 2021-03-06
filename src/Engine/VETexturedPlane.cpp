#include "VETexturedPlane.h"
#include "VECamera.h"
#include <glm/detail/type_mat.hpp>
#include <glm/detail/type_mat.hpp>

namespace VE
{
	TexturedPlane::TexturedPlane()
	{
		m_cmds = VEngine::getEngine()->getCmds();
		m_renderer = VEngine::getEngine()->getRenderer();

		cam = VEngine::getEngine()->getCamera();
	}

	TexturedPlane::~TexturedPlane()
	{
		delete cam;
	}

	void TexturedPlane::Create(std::string texturePath)
	{
		ShaderManager* mngr = ShaderManager::getShaderManager();
		mngr->CreateProgram("uniformbuffer", m_renderer, &prgm);

		std::vector<tr_descriptor> descriptors(3);
		descriptors[0].type = tr_descriptor_type_uniform_buffer;
		descriptors[0].count = 1;
		descriptors[0].binding = 0;
		descriptors[0].shader_stages = tr_shader_stage_vert;
		descriptors[1].type = tr_descriptor_type_texture;
		descriptors[1].count = 1;
		descriptors[1].binding = 1;
		descriptors[1].shader_stages = tr_shader_stage_frag;
		descriptors[2].type = tr_descriptor_type_sampler;
		descriptors[2].count = 1;
		descriptors[2].binding = 2;
		descriptors[2].shader_stages = tr_shader_stage_frag;
		tr_create_descriptor_set(m_renderer, descriptors.size(), descriptors.data(), &m_desc_set);

		tr_pipeline_settings pipeline_settings = {};
		pipeline_settings.primitive_topo = tr_primitive_topo_tri_list;
		pipeline_settings.depth_test = true;
		pipeline_settings.depth_write = true;
		tr_create_pipeline(m_renderer, prgm, &Vertex::getAttributeDescriptions(), m_desc_set, m_renderer->swapchain_render_targets[0], &pipeline_settings, &m_pipeline);

		std::vector<float> vertexData = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
			-0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f,
			0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f,
			0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f,
		};

		uint64_t vertexDataSize = sizeof(float) * vertexData.size();
		uint64_t vertexStride = sizeof(float) * 6;
		tr_create_vertex_buffer(m_renderer, vertexDataSize, true, vertexStride, &m_rect_vertex_buffer);
		memcpy(m_rect_vertex_buffer->cpu_mapped_address, vertexData.data(), vertexDataSize);

		std::vector<uint16_t> indexData = {
			0, 1, 2,
			0, 2, 3
		};

		uint64_t indexDataSize = sizeof(uint16_t) * indexData.size();
		tr_create_index_buffer(m_renderer, indexDataSize, true, tr_index_type_uint16, &m_rect_index_buffer);
		memcpy(m_rect_index_buffer->cpu_mapped_address, indexData.data(), indexDataSize);

		int image_width = 0;
		int image_height = 0;
		int image_channels = 0;
		stbi_uc* image_data = stbi_load(texturePath.c_str(), &image_width, &image_height, &image_channels, 0);
		assert(NULL != image_data);
		uint32_t image_row_stride = image_width * image_channels;
		tr_create_texture_2d(m_renderer, image_width, image_height, tr_sample_count_1, tr_format_r8g8b8a8_unorm, tr_max_mip_levels, nullptr, false, tr_texture_usage_sampled_image, &m_texture);
		tr_util_update_texture_uint8(m_renderer->graphics_queue, image_width, image_height, image_row_stride, image_data, image_channels, m_texture, nullptr, nullptr);
		stbi_image_free(image_data);

		tr_create_sampler(m_renderer, &m_sampler);

		tr_create_uniform_buffer(m_renderer, 16 * sizeof(float), true, &m_uniform_buffer);

		m_desc_set->descriptors[0].uniform_buffers[0] = m_uniform_buffer;
		m_desc_set->descriptors[1].textures[0] = m_texture;
		m_desc_set->descriptors[2].samplers[0] = m_sampler;
		tr_update_descriptor_set(m_renderer, m_desc_set);
	}

	void TexturedPlane::Draw(tr_cmd* cmd)
	{
		glm::mat4 modl = glm::mat4(1.0f);
		glm::mat4 mvp = cam->getProjectionMatrix() * cam->getViewMatrix() * modl;
		memcpy(m_uniform_buffer->cpu_mapped_address, &mvp, sizeof(mvp));
		tr_cmd_bind_pipeline(cmd, m_pipeline);
		tr_cmd_bind_index_buffer(cmd, m_rect_index_buffer);
		tr_cmd_bind_vertex_buffers(cmd, 1, &m_rect_vertex_buffer);
		tr_cmd_bind_descriptor_sets(cmd, m_pipeline, m_desc_set);
		tr_cmd_draw_indexed(cmd, m_rect_index_buffer->size, 0);
	}

	void TexturedPlane::Update()
	{
	}

	void TexturedPlane::setProgram(tr_shader_program program)
	{
	}

	void TexturedPlane::Destroy()
	{
	}

	void TexturedPlane::Rotate(glm::vec3 rotation)
	{
	}

	void TexturedPlane::MoveBy(glm::vec3 movement, bool local)
	{
	}
}
