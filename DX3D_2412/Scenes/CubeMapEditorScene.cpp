#include "Framework.h"
#include "CubeMapEditorScene.h"

CubeMapEditorScene::CubeMapEditorScene()
{
	mapEditor = new CubeMapEditor();
	UIManager::Get();
}

CubeMapEditorScene::~CubeMapEditorScene()
{
	delete mapEditor;
	ClickerUIManager::Delete();
}

void CubeMapEditorScene::Update()
{
	mapEditor->Update();
	ClickerUIManager::Get()->Update();
}

void CubeMapEditorScene::PreRender()
{
	
}

void CubeMapEditorScene::Render()
{
	mapEditor->Render();
}

void CubeMapEditorScene::PostRender()
{
	ClickerUIManager::Get()->Render();
}

void CubeMapEditorScene::GUIRender()
{
	mapEditor->Edit();	
	ClickerUIManager::Get()->Edit();
}
