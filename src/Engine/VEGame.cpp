#include "VEGame.h"

namespace VE
{
	void Game::Create()
	{
		VEngine* engine = VEngine::getEngine();
		scene_manager = reinterpret_cast<SceneManager*>(engine->getSceneManager());
		input = engine->getInput();
	}

	Input* Game::getInput()
	{
		return input;
	}
}
