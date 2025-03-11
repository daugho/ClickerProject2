#pragma once

class Environment : public Singleton<Environment>
{
private:
    friend class Singleton;

    Environment();
    ~Environment();

public:
    void Update();

    void SetRender();
    void SetPostRender();
    void Edit();

    Camera* GetMainCamera() { return mainCamera; }

    LightBuffer::Light* GetLight(UINT index) { return &lightBuffer->Get().lights[index]; }

    void SetAlphaBlend(bool isAlpha);
    void SetAdditive();

private:
    void CreateState();
    void CreateUIBuffer();

    void EditLight(LightBuffer::Light* light, int index);

public:

private:
    Camera* mainCamera;

    ViewBuffer* uiViewBuffer;
    MatrixBuffer* uiProjectionBuffer;
    LightBuffer* lightBuffer;

    SamplerState* samplerState;
    RasterizerState* rasterizerState[2];
    BlendState* blendState[2];
    DepthStencilState* depthStencilState[2];

    bool isWireFrame = false;
};