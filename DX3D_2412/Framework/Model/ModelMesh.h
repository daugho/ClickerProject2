#pragma once

class ModelMesh
{
public:
    ModelMesh(string name);
    ~ModelMesh();

    void Render();
    void RenderInstanced(UINT count);

    void CreateMesh(void* vertices, UINT vertexCount,
        void* indices, UINT indexCount);

    void SetMaterial(Material* material, UINT index)
    {
        this->material = material;
        data.materialIndex = index;
    }

    MeshData GetData() { return data; }

private:
    MeshData data;

    Material* material;
    Mesh<ModelVertex>* mesh;
};