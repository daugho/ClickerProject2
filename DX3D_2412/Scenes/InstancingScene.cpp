#include "Framework.h"
#include "InstancingScene.h"

InstancingScene::InstancingScene()
{
    //quads.resize(SIZE);
    //
    //for (Quad*& quad : quads)
    //{
    //    quad = new Quad(Vector2(1, 1));
    //    quad->GetMaterial()->SetDiffuseMap(L"Resources/Textures/Landscape/Box.png");
    //    Vector3 pos(GameMath::Random(-10.0f, 10.0f), GameMath::Random(-10.0f, 10.0f));
    //    quad->SetLocalPosition(pos);        
    //    quad->UpdateWorld();
    //}

    quad = new Quad(Vector2(1, 1));
    quad->GetMaterial()->SetShader(L"Instancing/UnlitInstancing.hlsl");
    quad->GetMaterial()->SetDiffuseMap(L"Resources/Textures/Landscape/Box.png");

    instanceData.resize(SIZE);
    for (Matrix& transform : instanceData)
    {
        Vector3 pos(GameMath::Random(-10.0f, 10.0f), GameMath::Random(-10.0f, 10.0f));

        transform = XMMatrixTranslation(pos.x, pos.y, pos.z);
        transform = XMMatrixTranspose(transform);
    }

    instanceBuffer = new VertexBuffer(instanceData.data(), sizeof(Matrix), SIZE);
}

InstancingScene::~InstancingScene()
{
    //for (Quad*& quad : quads)
    //    delete quad;

    delete quad;
    delete instanceBuffer;
}

void InstancingScene::Update()
{
}

void InstancingScene::PreRender()
{
}

void InstancingScene::Render()
{
    //for (Quad*& quad : quads)
    //    quad->Render();

    instanceBuffer->Set(1);

    quad->RenderInstanced(SIZE);
}

void InstancingScene::PostRender()
{
}

void InstancingScene::GUIRender()
{
}
