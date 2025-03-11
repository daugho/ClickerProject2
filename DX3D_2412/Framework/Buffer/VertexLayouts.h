#pragma once

//Vertex(정점) : 3차원 공간에서의 한점
struct Vertex
{
    Float3 pos;

    Vertex(float x = 0.0f, float y = 0.0f, float z = 0.0f)
        : pos(x, y, z)
    {
    }
};

struct VertexColor
{
    Float3 pos;
    Float4 color;
        
    VertexColor(float x = 0.0f, float y = 0.0f, float z = 0.0f,
        float r = 1.0f, float g = 1.0f, float b = 1.0f)
        : pos(x, y, z), color(r, g, b, 1)
    {
    }
};

struct VertexUV
{
    Float3 pos = {};
    Float2 uv = {};

    VertexUV() = default;
    VertexUV(float x, float y, float z, float u, float v)
        : pos(x, y, z), uv(u, v)
    {
    }
};

struct VertexUVNormal
{
    Float3 pos = {};
    Float2 uv = {};
    Float3 normal = {};

    VertexUVNormal() = default;
    VertexUVNormal(float x, float y, float z, float u, float v)
        : pos(x, y, z), uv(u, v)
    {
    }
};

struct VertexUVNormalTangent
{
    Float3 pos = {};
    Float2 uv = {};
    Float3 normal = {};
    Float3 tangent = {};


    VertexUVNormalTangent() = default;
    VertexUVNormalTangent(float x, float y, float z, float u, float v)
        : pos(x, y, z), uv(u, v)
    {
    }
};

struct VertexUVNormalTangentBlend
{
    Float3 pos = {};
    Float2 uv = {};
    Float3 normal = {};
    Float3 tangent = {};
    Float4 indices = {};
    Float4 weights = {};

    VertexUVNormalTangentBlend() = default;
    VertexUVNormalTangentBlend(float x, float y, float z, float u, float v)
        : pos(x, y, z), uv(u, v)
    {
    }
};