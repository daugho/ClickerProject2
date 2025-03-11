#include "Framework.h"

ModelInstancing::ModelInstancing(string name, UINT size)
    : Model(name), size(size)
{
    SetShader(L"Instancing/ModelInstancing.hlsl");
    instanceDatas = new Matrix[size];
    instanceBuffer = new VertexBuffer(instanceDatas, sizeof(Matrix), size);
}

ModelInstancing::~ModelInstancing()
{
    delete[] instanceDatas;
    delete instanceBuffer;

    for (Transform* transform : transforms)
        delete transform;

    transforms.clear();
}

void ModelInstancing::Update()
{
    drawCount = 0;

    FOR(transforms.size())
    {
        if (transforms[i]->IsActive())
        {
            transforms[i]->UpdateWorld();
            instanceDatas[drawCount] = XMMatrixTranspose(transforms[i]->GetWorld());

            drawCount++;
        }
    }

    instanceBuffer->Update(instanceDatas, drawCount);
}

void ModelInstancing::Render()
{
    instanceBuffer->Set(1);

    for (ModelMesh* mesh : meshes)
        mesh->RenderInstanced(drawCount);
}

void ModelInstancing::Edit()
{
    ImGui::Text("DrawCount : %d", drawCount);

    if (ImGui::Button("Add"))
        Add();

    for (Transform* transform : transforms)
        transform->Edit();

    Model::Edit();
}

Transform* ModelInstancing::Add()
{
    if (transforms.size() >= size)
        return nullptr;

    Transform* transform = new Transform();
    transform->SetTag(name + "_" + to_string(transforms.size()));
    transforms.push_back(transform);

    return transform;
}
