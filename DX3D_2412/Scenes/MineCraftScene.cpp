#include "Framework.h"
#include "MineCraftScene.h"

MineCraftScene::MineCraftScene()
{
	BlockManager::Get()->CreateBlocks(10, 10);
	UIManager::Get();

	player = new Player();
	player->SetLocalPosition(5, 5, 5);

	CAM->SetTarget(player);
	CAM->TargetOptionLoad("FPSMode");

	skybox = new Skybox(L"Resources/Textures/Landscape/Snow_ENV.dds");
}

MineCraftScene::~MineCraftScene()
{
	BlockManager::Delete();
	UIManager::Delete();

	delete player;
}

void MineCraftScene::Update()
{
	if (KEY->Down(VK_F2))
	{
		CAM->SetTarget(nullptr);
	}

	if (KEY->Down(VK_F3))
	{
		CAM->SetTarget(player);
		CAM->TargetOptionLoad("FPSMode");
	}

	if (KEY->Down(VK_F4))
	{
		CAM->SetTarget(player);
		CAM->TargetOptionLoad("QuaterViewMode");
	}

	BlockManager::Get()->Update();
	UIManager::Get()->Update();

	player->Update();
}

void MineCraftScene::PreRender()
{
}

void MineCraftScene::Render()
{
	skybox->Render();

	BlockManager::Get()->Render();

	player->Render();	
}

void MineCraftScene::PostRender()
{
	//BlockManager::Get()->PostRender();
	UIManager::Get()->Render();
	player->PostRender();	
}

void MineCraftScene::GUIRender()
{
	UIManager::Get()->Edit();
}
