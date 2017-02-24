#include "VEGameObject.h"

namespace VE
{
	void GameObject::Draw(tr_cmd* cmd)
	{
	}

	void GameObject::Update()
	{
	}

	void GameObject::setProgram(tr_shader_program program)
	{
		this->shader_program = program;
	}

	void GameObject::Destroy()
	{
	}

	void GameObject::Rotate(glm::vec3 rotation)
	{
		yaw += rotation.x;
		pitch += rotation.y;
		roll += rotation.z;
	}

	void GameObject::MoveBy(glm::vec3 movement, bool local)
	{
		if(local)
		{
			
		}
		else
		{
			position += movement;
		}
	}

	glm::vec3 GameObject::getRotation() const
	{
		return glm::vec3(yaw, pitch, roll);
	}

	glm::vec3 GameObject::getPosition() const
	{
		return position;
	}

	void GameObject::setRotation(glm::vec3 rotation)
	{
		yaw = rotation.x;
		pitch = rotation.y;
		roll = rotation.z;
	}

	GameObject* GameObject::getObject()
	{
		return this;
	}

}