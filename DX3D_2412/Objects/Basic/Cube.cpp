#include "Framework.h"

Cube::Cube(Vector3 size, Vector2 tiling) : size(size), tiling(tiling)
{
    MakeMesh();
    MakeNormal();
    MakeTangent();
    mesh->CreateMesh();    

    collider = new BoxCollider(size);
    collider->SetParent(this);
}

void Cube::Update()
{
    if (!IsActive()) return;

    UpdateWorld();
    collider->UpdateWorld();
}

void Cube::Render()
{
    if (!IsActive()) return;

    GameObject::Render();
    collider->Render();
}

void Cube::MakeMesh()
{
    //Polygon : 3차원에서의 정점 3개로 이루어진 삼각형
    //-> 시계방향을 앞면으로 해서 앞면만 출력함.

    SetVertices();

    vector<UINT>& indices = mesh->GetIndices();
    indices = {
        //Front
        0, 1, 2, 2, 1, 3,
        //Right
        4, 5, 6, 6, 5, 7,
        //Up
        8, 9, 10, 10, 9, 11,
        //Back
        12, 13, 14, 14, 13, 15,
        //Left
        16, 17, 18, 18, 17, 19,
        //Down
        20, 21, 22, 22, 21, 23
    };
}

void Cube::SetVertices()
{
    vector<VertexType>& vertices = mesh->GetVertices();
    vertices.clear();
    //Front
    vertices.emplace_back(-0.5f * size.x, +0.5f * size.y, -0.5f * size.z, 0, 0);//0
    vertices.emplace_back(+0.5f * size.x, +0.5f * size.y, -0.5f * size.z, tiling.x, 0);//1
    vertices.emplace_back(-0.5f * size.x, -0.5f * size.y, -0.5f * size.z, 0, tiling.y);//2
    vertices.emplace_back(+0.5f * size.x, -0.5f * size.y, -0.5f * size.z, tiling.x, tiling.y);//3

    //Right
    vertices.emplace_back(+0.5f * size.x, +0.5f * size.y, -0.5f * size.z, 0, 0);//1
    vertices.emplace_back(+0.5f * size.x, +0.5f * size.y, +0.5f * size.z, tiling.x, 0);//5
    vertices.emplace_back(+0.5f * size.x, -0.5f * size.y, -0.5f * size.z, 0, tiling.y);//3
    vertices.emplace_back(+0.5f * size.x, -0.5f * size.y, +0.5f * size.z, tiling.x, tiling.y);//7

    //Up
    vertices.emplace_back(-0.5f * size.x, +0.5f * size.y, +0.5f * size.z, 0, 0);//4
    vertices.emplace_back(+0.5f * size.x, +0.5f * size.y, +0.5f * size.z, tiling.x, 0);//5
    vertices.emplace_back(-0.5f * size.x, +0.5f * size.y, -0.5f * size.z, 0, tiling.y);//0
    vertices.emplace_back(+0.5f * size.x, +0.5f * size.y, -0.5f * size.z, tiling.x, tiling.y);//1

    //Back
    vertices.emplace_back(-0.5f * size.x, +0.5f * size.y, +0.5f * size.z, 0, 0);//4    
    vertices.emplace_back(-0.5f * size.x, -0.5f * size.y, +0.5f * size.z, 0, tiling.y);//6
    vertices.emplace_back(+0.5f * size.x, +0.5f * size.y, +0.5f * size.z, tiling.x, 0);//5
    vertices.emplace_back(+0.5f * size.x, -0.5f * size.y, +0.5f * size.z, tiling.x, tiling.y);//7

    //Left
    vertices.emplace_back(-0.5f * size.x, +0.5f * size.y, -0.5f * size.z, 0, 0);//0    
    vertices.emplace_back(-0.5f * size.x, -0.5f * size.y, -0.5f * size.z, 0, tiling.y);//2
    vertices.emplace_back(-0.5f * size.x, +0.5f * size.y, +0.5f * size.z, tiling.x, 0);//4
    vertices.emplace_back(-0.5f * size.x, -0.5f * size.y, +0.5f * size.z, tiling.x, tiling.y);//6

    //Down
    vertices.emplace_back(-0.5f * size.x, -0.5f * size.y, +0.5f * size.z, 0, 0);//6    
    vertices.emplace_back(-0.5f * size.x, -0.5f * size.y, -0.5f * size.z, 0, tiling.y);//2
    vertices.emplace_back(+0.5f * size.x, -0.5f * size.y, +0.5f * size.z, tiling.x, 0);//7
    vertices.emplace_back(+0.5f * size.x, -0.5f * size.y, -0.5f * size.z, tiling.x, tiling.y);//3

    //MakeNormal();
    //MakeTangent();
}

//void Cube::Update()
//{
//    if (GetAsyncKeyState('D'))
//        pos.x += 0.001f;
//    if (GetAsyncKeyState('A'))
//        pos.x -= 0.001f;
//
//    Matrix T = XMMatrixTranslation(pos.x, pos.y, pos.z);
//
//
//    if (GetAsyncKeyState('T'))
//        scale.y += 0.001f;
//    if (GetAsyncKeyState('G'))
//        scale.y -= 0.001f;
//
//    Matrix S = XMMatrixScaling(scale.x, scale.y, scale.z);
//
//    static float angle = 0.0f;
//
//    if (GetAsyncKeyState(VK_UP))
//        rot.z += 0.001f;
//    if (GetAsyncKeyState(VK_DOWN))
//        rot.z -= 0.001f;
//
//    worldMat._11 = cos(angle);
//    worldMat._13 = sin(angle);
//    worldMat._31 = -sin(angle);
//    worldMat._33 = cos(angle);
//
//    Matrix R = XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z);
//
//    //world = XMLoadFloat4x4(&worldMat);
//    world = S * R * T;
//}