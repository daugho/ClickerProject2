#include "Framework.h"
#include "PacmanScene.h"

PacmanScene::PacmanScene()
{
	MapManager::Get()->Load("Resources/TextData/Pacman.map");
}

PacmanScene::~PacmanScene()
{
	MapManager::Delete();
}

void PacmanScene::Update()
{
	MapManager::Get()->Update();
}

void PacmanScene::PreRender()
{
}

void PacmanScene::Render()
{
	MapManager::Get()->Render();
}

void PacmanScene::PostRender()
{
}

void PacmanScene::GUIRender()
{
}
