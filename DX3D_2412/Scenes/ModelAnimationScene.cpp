#include "Framework.h"
#include "ModelAnimationScene.h"

ModelAnimationScene::ModelAnimationScene()
{
	model = new ModelAnimator("Ghost");	
	model->SetShader(L"Model/Model.hlsl");
	FOR(6)
		model->ReadClip("Ghost_animation", i);
	//model->ReadClip("Idle");
	//model->ReadClip("Run");
	//model->ReadClip("Attack");
	model->CreateTexture();
}

ModelAnimationScene::~ModelAnimationScene()
{
	delete model;
}

void ModelAnimationScene::Update()
{
	if (KEY->Down('1'))
	{
		model->PlayClip(0);
	}
	if (KEY->Down('2'))
	{
		model->PlayClip(1);
	}
	if (KEY->Down('3'))
	{
		model->PlayClip(2);
	}
	if (KEY->Down('4'))
	{
		model->PlayClip(3);
	}

	model->Update();
}

void ModelAnimationScene::PreRender()
{
}

void ModelAnimationScene::Render()
{
	model->Render();
}

void ModelAnimationScene::PostRender()
{
}

void ModelAnimationScene::GUIRender()
{
	model->Edit();
}
