#include "Framework.h"

#include "Scenes/TutorialScene.h"
#include "Scenes/SolaSystemScene.h"
#include "Scenes/SphereScene.h"
#include "Scenes/GridScene.h"
#include "Scenes/SpawnScene.h"
#include "Scenes/CollisionScene.h"
#include "Scenes/ShootingScene.h"
#include "Scenes/MineCraftScene.h"
#include "Scenes/LightScene.h"
#include "Scenes/TerrainScene.h"
#include "Scenes/CubeMapEditorScene.h"
#include "Scenes/PacmanScene.h"
#include "Scenes/ModelRenderScene.h"
#include "Scenes/ModelExportScene.h"
#include "Scenes/InstancingScene.h"
#include "Scenes/ModelInstancingScene.h"
#include "Scenes/ModelAnimationScene.h"

GameManager::GameManager()
{
	SCENE->Create("Grid", new GridScene());
	//SCENE->Create("Export", new ModelExportScene());
	//SCENE->Create("Start", new SphereScene());
	//SCENE->Create("Start", new SpawnScene());
	//SCENE->Create("Start", new CollisionScene());
	//SCENE->Create("Start", new ShootingScene());
	//SCENE->Create("Start", new MineCraftScene());
	//SCENE->Create("Start", new LightScene());
	//SCENE->Create("Start", new TerrainScene());
	SCENE->Create("Start", new CubeMapEditorScene());
	//SCENE->Create("Start", new PacmanScene());	
	//SCENE->Create("Start", new ModelRenderScene());
	//SCENE->Create("Start", new InstancingScene());
	//SCENE->Create("Start", new ModelInstancingScene());
	//SCENE->Create("Start", new ModelAnimationScene());
	

	SCENE->Add("Grid");
	SCENE->Add("Start");

	Create();
}

GameManager::~GameManager()
{
	Release();
}

void GameManager::Update()
{
	Keyboard::Get()->Update();
	Timer::Get()->Update();

	SCENE->Update();

	Environment::Get()->Update();
}

void GameManager::Render()
{
	SCENE->PreRender();

	Device::Get()->Clear();

	Environment::Get()->SetRender();
	SCENE->Render();

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	Font::Get()->GetDC()->BeginDraw();

	string fps = "FPS : " + to_string(Timer::Get()->GetFPS());
	Font::Get()->RenderText(fps, { 100, SCREEN_HEIGHT - 10 });

	Environment::Get()->SetPostRender();
	SCENE->PostRender();

	Environment::Get()->Edit();
	SCENE->GUIRender();

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	Font::Get()->GetDC()->EndDraw();

	Device::Get()->Present();
}

void GameManager::Create()
{
	Device::Get();
	Environment::Get();
	Timer::Get();
	Keyboard::Get();
	SceneManager::Get();
	Font::Get();
	Observer::Get();

	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(DEVICE, DC);

	Font::Get()->AddColor("White", 1, 1, 1);
	Font::Get()->AddStyle("Default", L"µ¸¿òÃ¼");

	Font::Get()->SetColor("White");
	Font::Get()->SetStyle("Default");
}

void GameManager::Release()
{
	Device::Delete();	
	Environment::Delete();
	Timer::Delete();
	Keyboard::Delete();
	SceneManager::Delete();
	Font::Delete();
	Observer::Delete();

	Shader::Delete();
	Texture::Delete();

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();

	ImGui::DestroyContext();
}

