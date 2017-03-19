#pragma once
#include <vector>
#include "VEGameObject.h"
#include "VEngine.hpp"

namespace VE
{
	class Scene
	{
	public:
		virtual void addObject(GameObject* object) final;
		virtual void Draw(tr_cmd* cmd, float dt) final;
		virtual void Update(float dt) = 0;
	protected:
		Camera* cam = nullptr;
	private:
		std::vector<GameObject*> objects;
	};
}
