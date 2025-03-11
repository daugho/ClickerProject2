#include "Framework.h"

Model::Model(string name) : name(name)
{
    SetTag(name);

    ReadMaterial();
    ReadMesh();

    worldBuffer = new MatrixBuffer();

}

Model::~Model()
{
    for (Material* material : materials)
        delete material;

    for (ModelMesh* mesh : meshes)
        delete mesh;

    delete worldBuffer;
   //delete collider;
}

void Model::Render()
{
    worldBuffer->Set(world);
    worldBuffer->SetVS(0);

    Environment::Get()->SetAlphaBlend(true);

    for (ModelMesh* mesh : meshes)
        mesh->Render();

    Environment::Get()->SetAlphaBlend(false);


}

void Model::Edit()
{
    Transform::Edit();

    for (Material* material : materials)
        material->Edit();
}

void Model::SetShader(wstring file)
{
    for (Material* material : materials)
        material->SetShader(file);
}

void Model::SetVertexShader(wstring file)
{
    for (Material* material : materials)
        material->SetVertexShader(file);
}

void Model::SetPixelShader(wstring file)
{
    for (Material* material : materials)
        material->SetPixelShader(file);
}

void Model::ReadMaterial()
{
    string file = "Resources/Models/Materials/" + name + "/" + name + ".mats";

    BinaryReader* reader = new BinaryReader(file);
    if (reader->IsFailed())
        assert(false);

    UINT size = reader->Data<UINT>();

    materials.reserve(size);

    for (UINT i = 0; i < size; i++)
    {
        Material* material = new Material();
        material->Load(reader->String());

        materials.push_back(material);
    }

    delete reader;
}

void Model::ReadMesh()
{
    string file = "Resources/Models/Meshes/" + name + ".mesh";

    BinaryReader* reader = new BinaryReader(file);
    if (reader->IsFailed())
        assert(false);

    UINT size = reader->Data<UINT>();

    meshes.reserve(size);

    for (UINT i = 0; i < size; i++)
    {
        ModelMesh* mesh = new ModelMesh(reader->String());
        UINT materialIndex = reader->Data<UINT>();
        mesh->SetMaterial(materials[materialIndex], materialIndex);

        UINT vertexCount = reader->Data<UINT>();
        ModelVertex* vertices = new ModelVertex[vertexCount];
        reader->Byte((void**)&vertices, sizeof(ModelVertex) * vertexCount);

        UINT indexCount = reader->Data<UINT>();
        UINT* indices = new UINT[indexCount];
        reader->Byte((void**)&indices, sizeof(UINT) * indexCount);

        mesh->CreateMesh(vertices, vertexCount, indices, indexCount);

        meshes.push_back(mesh);
    }

    size = reader->Data<UINT>();
    nodes.resize(size);
    for (NodeData& node : nodes)
    {
        node.index = reader->Data<int>();
        node.name = reader->String();
        node.parent = reader->Data<int>();
        node.transform = reader->Data<Matrix>();
    }

    size = reader->Data<UINT>();
    bones.resize(size);
    for (BoneData& bone : bones)
    {
        bone.index = reader->Data<int>();
        bone.name = reader->String();
        bone.offset = reader->Data<Matrix>();

        boneMap[bone.name] = bone.index;
    }

    delete reader;
}
