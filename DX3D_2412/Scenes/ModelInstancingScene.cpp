#include "Framework.h"
#include "ModelInstancingScene.h"

ModelInstancingScene::ModelInstancingScene()
{
    model = new ModelInstancing("FemaleA", 100);

    for (float z = 0; z < 10; z++)
    {
        for (float x = 0; x < 10; x++)
        {
            Transform* transform = model->Add();
            transform->SetLocalPosition(x, 0, z);
            transform->SetLocalScale(0.1f, 0.1f, 0.1f);
        }
    }
    model->Update();
}

ModelInstancingScene::~ModelInstancingScene()
{
}

void ModelInstancingScene::Update()
{
    model->Update();
}

void ModelInstancingScene::PreRender()
{
}

void ModelInstancingScene::Render()
{
    model->Render();
}

void ModelInstancingScene::PostRender()
{
}

void ModelInstancingScene::GUIRender()
{
    model->Edit();
}
