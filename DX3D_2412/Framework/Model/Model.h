#pragma once

class CapsuleCollider;

class Collider;

struct RaycastHit;

class Model : public Transform
{
public:
    Model(string name);
    ~Model();

    void Render();
    void Edit();

    void SetShader(wstring file);
    void SetVertexShader(wstring file);
    void SetPixelShader(wstring file);
   
    vector<Material*>& GetMaterials() { return materials; }

private:
    void ReadMaterial();
    void ReadMesh();

protected:
    string name;
    vector<Material*> materials;
	vector<ModelMesh*> meshes;
	vector<NodeData> nodes;
	vector<BoneData> bones;

    MatrixBuffer* worldBuffer;

    vector<Matrix> boneTransforms;
    map<string, Matrix> nodeTransforms;
    map<string, UINT> boneMap;

};