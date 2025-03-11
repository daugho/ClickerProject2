#include "Framework.h"
#include "TutorialScene.h"

TutorialScene::TutorialScene()
{
	cube = new Cube();
	cube->GetMaterial()->SetDiffuseMap(L"Resources/Textures/Landscape/Tree.png");
}

TutorialScene::~TutorialScene()
{
	delete cube;
}

void TutorialScene::Update()
{
	if (Keyboard::Get()->Press(VK_RIGHT))
		cube->Rotate(Vector3::Up(), DELTA);
	if (Keyboard::Get()->Press(VK_LEFT))
		cube->Rotate(Vector3::Down(), DELTA);

	//cube->SetLocalRotation(0.0, XM_PI / 2, 0.0f);
	float angle = cube->GetLocalRotation().y;
	//float angle = XM_PI / 2;

	Vector3 dir(sin(angle), 0.0f, cos(angle));
	if (Keyboard::Get()->Press(VK_UP))
		cube->Translate(cube->GetForward() * DELTA);
	if (Keyboard::Get()->Press(VK_DOWN))
		cube->Translate(cube->GetBack() * DELTA);

	cube->UpdateWorld();
}

void TutorialScene::PreRender()
{
}

void TutorialScene::Render()
{
	cube->Render();
}

void TutorialScene::PostRender()
{
}

void TutorialScene::GUIRender()
{
	cube->Edit();
}
