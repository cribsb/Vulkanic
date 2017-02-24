#pragma once
#include <vector>
#include "VEGameObject.h"

namespace VE
{
	class Scene
	{
	public:
		void addObject(GameObject* object);
	private:
		std::vector<GameObject> objects;
	};
}