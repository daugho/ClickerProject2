#pragma once

typedef VertexUVNormalTangentBlend ModelVertex;

struct MeshData
{
    string name;
    UINT materialIndex;

    vector<ModelVertex> vertices;
    vector<UINT> indices;
};
struct NodeData
{
    int index;
    string name;
    int parent;
    Matrix transform;
};

struct BoneData
{
    int index;
    string name;
    Matrix offset;
};

struct VertexWeights
{
    UINT indices[4] = {};
    float weights[4] = {};

    void Add(const UINT& index, const float& weight)
    {
        for (int i = 0; i < 4; i++)
        {
            if (weights[i] == 0.0f)
            {
                indices[i] = index;
                weights[i] = weight;
                return;
            }
        }
    }

    void Normalize()
    {
        float sum = 0.0f;

        for (int i = 0; i < 4; i++)
            sum += weights[i];

        for (int i = 0; i < 4; i++)
            weights[i] /= sum;
    }
};

struct KeyVector
{
    float time;
    Float3 value;
};

struct KeyQuat
{
    float time;
    Float4 value;
};

struct KeyData
{
    vector<KeyVector> scales;
    vector<KeyQuat> rotations;
    vector<KeyVector> positions;
};

struct KeyTransform
{
    Float3 scale;
    Float4 rot;
    Float3 pos;
};

struct KeyFrame
{
    string boneName;
    vector<KeyTransform> transforms;
};

struct ClipNode
{
    aiString name;
    vector<KeyTransform> transforms;
};

struct Clip
{
    string name;

    UINT frameCount;
    float tickPerSecond;

    vector<KeyFrame*> keyFrames;
};