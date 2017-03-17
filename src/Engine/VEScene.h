#pragma once
#include <vector>
#include "VEGameObject.h"
#include "Helper.hpp"

namespace VE
{
	class Scene
	{
	public:
		void addObject(GameObject* object);
		void Draw(tr_cmd* cmd);
	private:
		std::vector<GameObject*> objects;
	};
}
