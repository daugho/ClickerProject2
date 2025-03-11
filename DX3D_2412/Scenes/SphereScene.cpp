#include "Framework.h"
#include "SphereScene.h"

SphereScene::SphereScene()
{
	sphere = new Sphere();
	sphere->GetMaterial()->SetDiffuseMap(L"Resources/Textures/Landscape/Box.png");
}

SphereScene::~SphereScene()
{
	delete sphere;
}

void SphereScene::Update()
{
	sphere->UpdateWorld();
}

void SphereScene::PreRender()
{
}

void SphereScene::Render()
{
	sphere->Render();
}

void SphereScene::PostRender()
{
}

void SphereScene::GUIRender()
{
	sphere->Edit();
}
