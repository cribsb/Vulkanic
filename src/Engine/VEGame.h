#pragma once
#include "VESceneManager.h"
#include "Helper.hpp"
#include "VEInput.h"


namespace VE
{
	class Game
	{
	public:
		virtual void Create() final;
		virtual void Initialize() = 0;
		virtual void Update() = 0;
		Input* getInput();
	protected:
		//Do not override these variables!
		SceneManager* scene_manager = nullptr;
		Input* input = nullptr;
	};
}
