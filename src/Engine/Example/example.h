#pragma once
#include "../VEInput.h"
#include "../VEGame.h"
#include "../VE3dmodel.h"

class ExampleGame : public VE::Game
{
public:
	~ExampleGame() override;
	void Initialize() override;
};

class ExampleScene : public VE::Scene
{
public:
	ExampleScene();
	void Update(float dt) override;
	VE::Input* input;

private:
	VE::TexturedPlane* plane = nullptr;
	VE::DModel* model = nullptr;
};