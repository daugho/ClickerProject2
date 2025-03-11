#include "Framework.h"

ModelMesh::ModelMesh(string name)
{
	data.name = name;
}

ModelMesh::~ModelMesh()
{
	delete mesh;
}

void ModelMesh::Render()
{
	material->Set();
	mesh->Draw();
}

void ModelMesh::RenderInstanced(UINT count)
{
    material->Set();
    mesh->DrawInstanced(count);
}

void ModelMesh::CreateMesh(void* vertices, UINT vertexCount, void* indices, UINT indexCount)
{
    mesh = new Mesh<ModelVertex>();

    mesh->GetVertices().resize(vertexCount);
    memcpy(mesh->GetVertices().data(), vertices, sizeof(ModelVertex) * vertexCount);

    mesh->GetIndices().resize(indexCount);
    memcpy(mesh->GetIndices().data(), indices, sizeof(UINT) * indexCount);

    mesh->CreateMesh();

    data.vertices = mesh->GetVertices();
    data.indices = mesh->GetIndices();
}
