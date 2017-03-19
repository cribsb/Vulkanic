#pragma once
#include "VEGameObject.h"
#include "VEngine.hpp"
#include "VEShaderManager.h"
#include <glm/detail/type_mat.hpp>
#include <glm/detail/type_mat.hpp>


namespace VE
{
	class TexturedPlane : public GameObject
	{
	public:
		TexturedPlane();
		~TexturedPlane();

		void Create(std::string texturePath);
		virtual void Draw(tr_cmd* cmd) override final;
		virtual void Update() override final;
		virtual void setProgram(tr_shader_program program) override final;
		virtual void Destroy() override final;
		virtual void Rotate(glm::vec3 rotation) override final;
		virtual void MoveBy(glm::vec3 movement, bool local = true) override final;
	private:
		tr_descriptor_set* m_desc_set = nullptr;
		tr_shader_program* prgm = nullptr;
		tr_buffer* m_rect_index_buffer = nullptr;
		tr_buffer* m_rect_vertex_buffer = nullptr;
		tr_pipeline* m_pipeline = nullptr;
		tr_texture* m_texture = nullptr;
		tr_sampler* m_sampler = nullptr;
		tr_buffer* m_uniform_buffer = nullptr;
		tr_renderer* m_renderer = nullptr;
		tr_cmd** m_cmds = nullptr;

		Camera* cam = nullptr;
	};
}
