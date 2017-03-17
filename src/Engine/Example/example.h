#pragma once
#include "../VEGame.h"

class ExampleGame : public VE::Game
{
public:
	void Initialize() override;
	void Update() override;
};

class ExampleScene : public VE::Scene
{
public:
	ExampleScene();
	void Update();
	Input* input;
};