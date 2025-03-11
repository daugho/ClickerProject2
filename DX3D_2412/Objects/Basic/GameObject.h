#pragma once

template <typename T>
class GameObject : public Transform
{

public:
    GameObject();
    GameObject(wstring shaderFile);
    ~GameObject();    

    virtual void Update();
    void Render(D3D11_PRIMITIVE_TOPOLOGY type = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    void RenderInstanced(UINT count, D3D11_PRIMITIVE_TOPOLOGY type = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    Material* GetMaterial() { return material; }
    Mesh<T>* GetMesh() { return mesh; }
    void SetMesh(Mesh<T>* mesh) 
    {
        delete this->mesh;
        this->mesh = mesh; 
    }

protected:
    void MakeNormal();
    void MakeTangent();

protected:
    Mesh<T>* mesh;
    Material* material;

    MatrixBuffer* worldBuffer;
    friend class FloorManager;
    friend class Cube;
};

template<typename T>
inline GameObject<T>::GameObject()
{
    mesh = new Mesh<T>();
    material = new Material();
    worldBuffer = new MatrixBuffer();
}

template<typename T>
inline GameObject<T>::GameObject(wstring shaderFile)
{
    mesh = new Mesh<T>();
    material = new Material(shaderFile);
    worldBuffer = new MatrixBuffer();
}

template<typename T>
inline GameObject<T>::~GameObject()
{
    delete mesh;
    delete material;
    delete worldBuffer;
}

template<typename T>
inline void GameObject<T>::Update()
{
    UpdateWorld();
}

template<typename T>
inline void GameObject<T>::Render(D3D11_PRIMITIVE_TOPOLOGY type)
{
    if (!IsActive()) return;

    worldBuffer->Set(world);
    worldBuffer->SetVS(0);

    material->Set();
    mesh->Draw(type);
}

template<typename T>
inline void GameObject<T>::RenderInstanced(UINT count, D3D11_PRIMITIVE_TOPOLOGY type)
{
    if (!IsActive()) return;

    worldBuffer->Set(world);
    worldBuffer->SetVS(0);

    material->Set();
    mesh->DrawInstanced(count, type);
}

template<typename T>
inline void GameObject<T>::MakeNormal()
{
    vector<T>& vertices = mesh->GetVertices();
    vector<UINT>& indices = mesh->GetIndices();

    FOR(indices.size() / 3)
    {
        UINT index0 = indices[i * 3 + 0];
        UINT index1 = indices[i * 3 + 1];
        UINT index2 = indices[i * 3 + 2];

        Vector3 v0 = vertices[index0].pos;
        Vector3 v1 = vertices[index1].pos;
        Vector3 v2 = vertices[index2].pos;

        Vector3 e0 = v1 - v0;
        Vector3 e1 = v2 - v0;

        Vector3 normal = Vector3::Cross(e0, e1).GetNormalized();

        vertices[index0].normal += normal;
        vertices[index1].normal += normal;
        vertices[index2].normal += normal;
    }
}

template<typename T>
inline void GameObject<T>::MakeTangent()
{
    vector<T>& vertices = mesh->GetVertices();
    vector<UINT>& indices = mesh->GetIndices();

    FOR(indices.size() / 3)
    {
        UINT index0 = indices[i * 3 + 0];
        UINT index1 = indices[i * 3 + 1];
        UINT index2 = indices[i * 3 + 2];

        Vector3 p0 = vertices[index0].pos;
        Vector3 p1 = vertices[index1].pos;
        Vector3 p2 = vertices[index2].pos;

        Vector2 uv0 = vertices[index0].uv;
        Vector2 uv1 = vertices[index1].uv;
        Vector2 uv2 = vertices[index2].uv;

        Vector3 e0 = p1 - p0;
        Vector3 e1 = p2 - p0;

        float u0 = uv1.x - uv0.x;
        float u1 = uv2.x - uv0.x;
        float v0 = uv1.y - uv0.y;
        float v1 = uv2.y - uv0.y;

        float d = 1.0f / (u0 * v1 - v0 * u1);
        Vector3 tangent = d * (e0 * v1 - e1 * v0);

        vertices[index0].tangent += tangent;
        vertices[index1].tangent += tangent;
        vertices[index2].tangent += tangent;
    }
}
