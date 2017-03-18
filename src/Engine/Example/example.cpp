#include "example.h"

void ExampleGame::Initialize()
{
	VE::Scene* scene = new ExampleScene();
	scene_manager->addScene(scene, "scene");
	scene_manager->setCurrentScene(scene);
}

ExampleGame::~ExampleGame()
{
}

ExampleScene::ExampleScene()
{
	plane = new VE::TexturedPlane();
	plane->Create("assets/box_panel.jpg");
	model = new VE::DModel();
	model->Create("assets/teapot.obj", 0.001f);

	addObject(plane);
	addObject(model);
	input = VE::VEngine::getEngine()->getInput();
	cam = VE::VEngine::getEngine()->getCamera();
}

void ExampleScene::Update(float dt)
{
	if(input->IsKeyPressed(GLFW_KEY_W))
	{
		cam->MoveBy({0,0,-1*dt});
	}
	if(input->IsKeyPressed(GLFW_KEY_S))
	{
		cam->MoveBy({0,0,1*dt});
	}
	if(input->IsKeyPressed(GLFW_KEY_A))
	{
		cam->MoveBy({1*dt,0,0});
	}
	if(input->IsKeyPressed(GLFW_KEY_D))
	{
		cam->MoveBy({-1*dt,0,0});
	}
	if(input->IsKeyPressed(GLFW_KEY_ESCAPE))
	{
		VE::VEngine::Quit();
	}
}
