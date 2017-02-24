#pragma once
#include "../tinyvk.h"
#include <glm/glm.hpp>

namespace VE
{
	class GameObject
	{
	public:
		virtual ~GameObject() = 0;

		virtual void Draw(tr_cmd* cmd);
		virtual void Update();
		virtual void setProgram(tr_shader_program program);
		virtual void Destroy();
		virtual void Rotate(glm::vec3 rotation);
		virtual void MoveBy(glm::vec3 movement, bool local = true);
		virtual glm::vec3 getRotation() const;
		virtual glm::vec3 getPosition() const;
		virtual void setRotation(glm::vec3 rotation);

		GameObject* getObject();

	protected:
		tr_shader_program shader_program;
		/// \brief	The position.
		glm::vec3 position = {0.0f, 0.0f, 1.0f};
		///////////////////////////////////////////////////////////////////////
				/// \property	float pitch, yaw, roll
				///
				/// \brief	The rotation axis
				///////////////////////////////////////////////////////////////////////

		float pitch, yaw, roll;
	};
	inline GameObject::~GameObject()
	{
	}

}
