#include "Framework.h"
#include "ShootingScene.h"

ShootingScene::ShootingScene()
{
	enemy = new Enemy();
	player = new Player();

	plane = new Plane(Vector2(50, 50), 2, 2);
	plane->SetLocalPosition(-25, 0, -25);
	plane->UpdateWorld();
	plane->GetMaterial()->SetDiffuseMap(L"Resources/Textures/Landscape/Dirt2.png");
}

ShootingScene::~ShootingScene()
{
	delete enemy;
	delete player;
	delete plane;
}

void ShootingScene::Update()
{
	enemy->Update();
	player->Update();
}

void ShootingScene::PreRender()
{
}

void ShootingScene::Render()
{
	enemy->Render();
	player->Render();
	plane->Render();
}

void ShootingScene::PostRender()
{
	player->PostRender();
	enemy->PostRender();
}

void ShootingScene::GUIRender()
{
	player->Edit();
	BulletManager::Get()->Edit();
}
