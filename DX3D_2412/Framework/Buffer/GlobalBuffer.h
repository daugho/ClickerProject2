#pragma once

class MatrixBuffer : public ConstBuffer
{
public:
    MatrixBuffer() : ConstBuffer(&matrix, sizeof(Matrix))
    {
        matrix = XMMatrixIdentity();
    }

    void Set(Matrix value)
    {
        matrix = XMMatrixTranspose(value);
    }

private:
    Matrix matrix;
};

class ViewBuffer : public ConstBuffer
{
private:
    struct Data
    {
        Matrix view;
        Matrix invView;
    };

public:
    ViewBuffer() : ConstBuffer(&data, sizeof(Data))
    {
        data.view = XMMatrixIdentity();
        data.invView = XMMatrixIdentity();
    }

    void Set(Matrix view, Matrix invView)
    {
        data.view = XMMatrixTranspose(view);
        data.invView = XMMatrixTranspose(invView);
    }

private:
    Data data;
};

class MaterialBuffer : public ConstBuffer
{
public:
    struct Data
    {
        Float4 diffuse = { 1, 1, 1, 1 };        
        Float4 specular = { 1, 1, 1, 1 };
        Float4 ambient = { 1, 1, 1, 1 };
        Float4 emissive = { 0, 0, 0, 1 };

        float shininess = 24.0f;
        int hasNormalMap = 0;

        float padding[2];
    };


public:
    MaterialBuffer() : ConstBuffer(&data, sizeof(Data))
    {
    }

    Data* GetData() { return &data; }

private:
    Data data;
};

class FloatValueBuffer : public ConstBuffer
{
public:
    FloatValueBuffer() : ConstBuffer(values, sizeof(float) * 4)
    {
    }

    float* Get() { return values; }

private:
    float values[4] = {};
};

class LightBuffer : public ConstBuffer
{
public:
    struct Light
    {
        Float4 color = { 1, 1, 1, 1 };

        Float3 direction = { 0, -1, 1 };
        float attentionIntensity = 1.0f;

        Float3 position;
        float range = 10.0f;

        float inner = 55.0f;
        float outer = 70.0f;
        int type = 0;
        int isActive = 1;
    };

    struct Data
    {
        Light lights[MAX_LIGHT];

        Float4 ambientColor = { 0.1f, 0.1f, 0.1f, 1.0f };        
        Float4 ambientCeil = { 0.1f, 0.1f, 0.1f, 1.0f };

        int lightCount = 1;
        float padding[3];
    };

public:
    LightBuffer() : ConstBuffer(&data, sizeof(Data))
    {
    }

    Data& Get() { return data; }

private:
    Data data;
};