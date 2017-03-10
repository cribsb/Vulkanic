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
		currentScene = scene;
	}

	void SceneManager::setCurrentScene(std::string name)
	{
		currentScene = scenes.at(name);
	}

	Scene* SceneManager::getCurrentScene() const
	{
		if(currentScene != nullptr)
		{
			return currentScene;
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
		scenes.insert({name, scene});
	}
}