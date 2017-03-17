#include "VEInput.h"


namespace VE
{
	Input::Input(GLFWwindow* window)
	{
		this->window = window;
	}

	bool Input::IsKeyPressed(int key) const
	{
		bool ret = false;
		if(glfwGetKey(window, key) == GLFW_PRESS)
		{
			ret = true;
		}
		return ret;
	}

	bool Input::IsKeyDown(int key) const
	{
		bool ret = false;
		if(glfwGetKey(window, key) == GLFW_REPEAT)
		{
			ret = true;
		}
		return ret;
	}

	bool Input::IsKeyReleased(int key) const
	{
		bool ret = false;
		if(glfwGetKey(window, key) == GLFW_RELEASE)
		{
			ret = true;
		}
		return ret;
	}
}
