#include "example.h"

void ExampleGame::Initialize()
{
	VE::Scene* scene = new ExampleScene();
	scene_manager->addScene(scene, "scene");
	scene_manager->setCurrentScene(scene);
}

void ExampleGame::Update()
{
	
}

ExampleScene::ExampleScene()
{
	VE::TexturedPlane* plane = new VE::TexturedPlane();
	plane->Create("assets/box_panel.jpg");
	addObject(plane);
	input = reinterpret_cast<Input*>(VE::VEngine::getEngine()->getInput());
	
}

void ExampleScene::Update()
{
	
}
