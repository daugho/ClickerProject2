#include "Framework.h"
#include "GridScene.h"

GridScene::GridScene()
{
    material = new Material(L"Basic/Grid.hlsl");

    mesh = new Mesh<VertexColor>();
    MakeMesh();
    mesh->CreateMesh();

    worldBuffer = new MatrixBuffer();
}

GridScene::~GridScene()
{
    delete material;
    delete mesh;
    delete worldBuffer;
}

void GridScene::Update()
{
}

void GridScene::PreRender()
{
}

void GridScene::Render()
{
    worldBuffer->SetVS(0);

    material->Set();
    mesh->Draw(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
}

void GridScene::PostRender()
{
}

void GridScene::GUIRender()
{
}

void GridScene::MakeMesh()
{
    vector<VertexColor>& vertices = mesh->GetVertices();
    vertices.emplace_back(-MAX_SIZE, 0, 0, 1, 0, 0);
    vertices.emplace_back(+MAX_SIZE, 0, 0, 1, 0, 0);

    vertices.emplace_back(0, -MAX_SIZE, 0, 0, 1, 0);
    vertices.emplace_back(0, +MAX_SIZE, 0, 0, 1, 0);

    vertices.emplace_back(0, 0, -MAX_SIZE, 0, 0, 1);
    vertices.emplace_back(0, 0, +MAX_SIZE, 0, 0, 1);

    int halfSize = MAX_SIZE >> 1;

    for (int i = -halfSize; i <= halfSize; i++)
    {
        if (i == 0) continue;

        Vector3 color = { 0.2f, 0.2f, 0.2f };

        if (i % 10 == 0)
            color = { 0.8f, 0.8f, 0.8f };
        else if (i % 5 == 0)
            color = { 0.4f, 0.4f, 0.4f };

        vertices.emplace_back(i, 0, -halfSize, color.x, color.y, color.z);
        vertices.emplace_back(i, 0, +halfSize, color.x, color.y, color.z);

        vertices.emplace_back(-halfSize, 0, i, color.x, color.y, color.z);
        vertices.emplace_back(+halfSize, 0, i, color.x, color.y, color.z);
    }
}
