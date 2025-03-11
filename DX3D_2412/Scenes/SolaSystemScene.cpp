#include "Framework.h"
#include "SolaSystemScene.h"

SolaSystemScene::SolaSystemScene()
{
	planets.push_back(new Planet(1.0f));
	planets.push_back(new Planet(2.0f, 0.5f, 10.0f, planets[0]));
	planets.push_back(new Planet(5.0f, 2.5f, 5.0f, planets[1]));

	planets.front()->SetPivot(-1, -1, -1);
}

SolaSystemScene::~SolaSystemScene()
{
	for (Planet* planet : planets)
		delete planet;
}

void SolaSystemScene::Update()
{
	for (Planet* planet : planets)
		planet->Update();
}

void SolaSystemScene::PreRender()
{
}

void SolaSystemScene::Render()
{
	for (Planet* planet : planets)
		planet->Render();
}

void SolaSystemScene::PostRender()
{
}

void SolaSystemScene::GUIRender()
{
}
