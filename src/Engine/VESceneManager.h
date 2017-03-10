#pragma once

#include "VEScene.h"
#include <map>

namespace VE
{
	class SceneManager
	{
	public:
		SceneManager();
		~SceneManager();
		
		void setCurrentScene(Scene* scene);
		void setCurrentScene(std::string name);
		
		Scene* getCurrentScene() const;
		Scene* getScene(std::string name);
		std::map<std::string, Scene*> getScenes() const;
		void addScene(Scene* scene, std::string name);

	private:
		std::map<std::string, Scene*> scenes;
		Scene* currentScene = nullptr;
	};
}
