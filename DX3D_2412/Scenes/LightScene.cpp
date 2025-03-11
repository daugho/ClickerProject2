#include "Framework.h"
#include "LightScene.h"

LightScene::LightScene()
{
	FOR(1)
	{
		Sphere* sphere = new Sphere(3.0f);
		sphere->SetLocalPosition(i * 20, 0, 0);
		sphere->SetTag("Sphere" + to_string(i));
		sphere->Load();
		spheres.push_back(sphere);
	}

	plane = new Plane({20, 20}, 10, 10);
	plane->Load();
}

LightScene::~LightScene()
{
	for (Sphere* sphere : spheres)
		delete sphere;

	delete plane;
}

void LightScene::Update()
{
	for (Sphere* sphere : spheres)
		sphere->UpdateWorld();

	plane->UpdateWorld();
}

void LightScene::PreRender()
{
}

void LightScene::Render()
{
	for (Sphere* sphere : spheres)
		sphere->Render();

	plane->Render();
}

void LightScene::PostRender()
{
}

void LightScene::GUIRender()
{
	for (Sphere* sphere : spheres)
	{
		sphere->Edit();		
		sphere->GetMaterial()->Edit();
	}

	plane->Edit();
}
