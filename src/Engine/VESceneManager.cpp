#include "VESceneManager.h"

namespace VE
{

	SceneManager::SceneManager()
	{
	}

	SceneManager::~SceneManager()
	{
	}

	void SceneManager::setCurrentScene(Scene* scene)
	{
		current_scene = scene;
	}

	void SceneManager::setCurrentScene(std::string name)
	{
		current_scene = scenes.at(name);
	}

	Scene* SceneManager::getCurrentScene() const
	{
		if(current_scene != nullptr)
		{
			return current_scene;
		}
		else
		{
			return scenes.begin()->second;
		}
	}

	Scene* SceneManager::getScene(std::string name)
	{
		return scenes.at(name);
	}

	std::map<std::string, Scene*> SceneManager::getScenes() const
	{
		return scenes;
	}

	void SceneManager::addScene(Scene* scene, std::string name)
	{
		scenes[name] = scene;
	}
}