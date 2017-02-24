#include "VECamera.h"
#include <GLFW/glfw3.h>
#include <algorithm>

namespace VE
{
	Camera* Camera::camera;

	Camera::Camera()
	{
		setLens(75.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
		Rotate(glm::vec3(0, 0, 0)); //HACK ugly little hack
	}

	Camera::~Camera()
	{
	}

	glm::vec3 Camera::getPosition() const
	{
		return position;
	}

	void Camera::setPosition(const glm::vec3& pos)
	{
		position = pos;
	}

	glm::vec3 Camera::getRight() const
	{
		return right;
	}

	glm::vec3 Camera::getUp() const
	{
		return up;
	}

	glm::vec3 Camera::getLook() const
	{
		return look;
	}

	float Camera::getNearZ() const
	{
		return nearZ;
	}

	float Camera::getFarZ() const
	{
		return farZ;
	}

	float Camera::getAspectRatio() const
	{
		return aspectRatio;
	}

	float Camera::getFoV() const
	{
		return fov;
	}

	void Camera::setLens(float FoV, float aspectRatio, float nearZ, float farZ)
	{
		this->fov = FoV;
		this->aspectRatio = aspectRatio;
		this->nearZ = nearZ;
		this->farZ = farZ;
		glm::mat4x4 proj = glm::perspective(this->fov, this->aspectRatio, this->nearZ, this->farZ);
		projectionMatrix = proj;
		UpdateViewMatrix();
	}

	glm::mat4x4 Camera::getViewMatrix() const
	{
		return viewMatrix;
	}

	glm::mat4x4 Camera::getProjectionMatrix() const
	{
		return projectionMatrix;
	}

	void Camera::Rotate(glm::vec3 rotation)
	{
		//Y or yaw
		glm::mat4x4 r;
		r = glm::rotate(r, -rotation.y*0.1f, right);
		up = r * glm::vec4(up, 1.0f);
		look = r * glm::vec4(look, 1.0f);
		UpdateViewMatrix();
		//X or pitch
		r = glm::rotate(r, rotation.x*0.1f, glm::vec3(0, 1, 0));
		right = r * glm::vec4(right, 1.0f);
		up = r * glm::vec4(up, 1.0f);
		look = r * glm::vec4(look, 1.0f);

		UpdateViewMatrix();
	}

	void Camera::MoveBy(glm::vec3 movement, bool local)
	{
		if(local)
		{
			Walk(movement.z);
			Strafe(movement.x);
			UpdateViewMatrix();
		}
		else
		{
			position += movement;
		}
		UpdateViewMatrix();
	}

	void Camera::UpdateViewMatrix()
	{
		glm::vec3 r = right;
		glm::vec3 u = up;
		glm::vec3 l = look;
		glm::vec3 p = position;

		l = glm::normalize(l);
		u = glm::normalize(glm::cross(l, r));
		r = glm::cross(u, l);

		float x = -glm::dot(p, r);
		float y = -glm::dot(p, u);
		float z = -glm::dot(p, l);

		right = r;
		up = u;
		look = l;

		viewMatrix[0][0] = right.x;
		viewMatrix[1][0] = right.y;
		viewMatrix[2][0] = right.z;
		viewMatrix[3][0] = x;

		viewMatrix[0][1] = up.x;
		viewMatrix[1][1] = up.y;
		viewMatrix[2][1] = up.z;
		viewMatrix[3][1] = y;

		viewMatrix[0][2] = look.x;
		viewMatrix[1][2] = look.y;
		viewMatrix[2][2] = look.z;
		viewMatrix[3][2] = z;

		viewMatrix[0][3] = 0.0f;
		viewMatrix[1][3] = 0.0f;
		viewMatrix[2][3] = 0.0f;
		viewMatrix[3][3] = 1.0f;
	}

	Camera* Camera::getCamera()
	{
		if(camera == nullptr)
		{
			camera = new Camera();
		}
		return camera;
	}

	void Camera::Strafe(float d)
	{
		glm::vec3 s = glm::vec3(d);
		glm::vec3 r = right;
		glm::vec3 p = position;
		position = (s*r) + p;
	}

	void Camera::Walk(float d)
	{
		glm::vec3 s = glm::vec3(d);
		glm::vec3 l = look;
		glm::vec3 p = position;
		position = (s*l) + p;
	}
}
