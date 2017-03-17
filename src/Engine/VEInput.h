#pragma once
#include <map>
#include <queue>
#include <chrono>
#include <GLFW/glfw3.h>

namespace VE
{
	class Input
	{
	public:
		Input(GLFWwindow* window);

		bool IsKeyPressed(int key) const;
		bool IsKeyReleased(int key) const;
		bool IsKeyDown(int key) const;
	private:
		GLFWwindow* window = nullptr;
	};
}
