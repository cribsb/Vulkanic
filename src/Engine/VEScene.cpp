#include "VEScene.h"

namespace VE
{

	void Scene::addObject(GameObject* object)
	{
		objects.push_back(object);
	}

	void Scene::Draw(tr_cmd* cmd)
	{
		for(uint32_t i = 0; i < objects.size(); ++i)
		{
			objects[i]->Draw(cmd);
		}
	}
}