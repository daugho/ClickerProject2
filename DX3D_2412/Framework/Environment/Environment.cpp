#include "Framework.h"

Environment::Environment()
{
    mainCamera = new Camera();
    mainCamera->Load();

    lightBuffer = new LightBuffer();

    CreateState();
    CreateUIBuffer();
}

Environment::~Environment()
{
    delete samplerState;
    delete rasterizerState[0];
    delete rasterizerState[1];
    delete blendState[0];
    delete blendState[1];
    delete depthStencilState[0];
    delete depthStencilState[1];

    delete uiViewBuffer;
    delete uiProjectionBuffer;
    delete lightBuffer;

    mainCamera->Save();
}

void Environment::Update()
{
    if (KEY->Down(VK_OEM_3))
        isWireFrame = !isWireFrame;

    if (KEY->Down(VK_TAB))
        Collider::OnDraw();

    mainCamera->Update();
}

void Environment::SetRender()
{
    mainCamera->SetView();

    rasterizerState[isWireFrame]->SetState();
    blendState[0]->SetState();
    depthStencilState[0]->SetState();

    lightBuffer->SetPS(1);
}

void Environment::SetPostRender()
{
    uiViewBuffer->SetVS(1);
    uiProjectionBuffer->SetVS(2);

    blendState[1]->SetState();
    depthStencilState[1]->SetState();
}

void Environment::Edit()
{
    mainCamera->Edit();

    if (ImGui::TreeNode("LightOption"))
    {
        if (ImGui::Button("Add"))
            lightBuffer->Get().lightCount++;

        FOR(lightBuffer->Get().lightCount)
            EditLight(&lightBuffer->Get().lights[i], i);

        ImGui::ColorEdit3("AmbientColor", (float*)&lightBuffer->Get().ambientColor);
        ImGui::ColorEdit3("AmbientCeilColor", (float*)&lightBuffer->Get().ambientCeil);

        ImGui::TreePop();
    }
}

void Environment::SetAlphaBlend(bool isAlpha)
{
    blendState[1]->Alpha(isAlpha);
    blendState[1]->SetState();
}

void Environment::SetAdditive()
{
    blendState[1]->Additive();
    blendState[1]->SetState();
}

void Environment::CreateState()
{
    samplerState = new SamplerState();
    samplerState->SetState();

    rasterizerState[0] = new RasterizerState();
    rasterizerState[1] = new RasterizerState();
    rasterizerState[1]->FillMode(D3D11_FILL_WIREFRAME);    

    blendState[0] = new BlendState();
    blendState[1] = new BlendState();
    blendState[1]->Alpha(true);

    depthStencilState[0] = new DepthStencilState();
    depthStencilState[1] = new DepthStencilState();
    depthStencilState[1]->DepthEnable(false);
}

void Environment::CreateUIBuffer()
{
    uiViewBuffer = new ViewBuffer();
    uiProjectionBuffer = new MatrixBuffer();

    Matrix orthographic = XMMatrixOrthographicOffCenterLH(
        0.0f, SCREEN_WIDTH, 0.0f, SCREEN_HEIGHT, -1.0f, 1.0f);

    uiProjectionBuffer->Set(orthographic);
}

void Environment::EditLight(LightBuffer::Light* light, int index)
{
    string label = "Light_" + to_string(index);

    if (ImGui::TreeNode(label.c_str()))
    {        
        ImGui::Checkbox("Active", (bool*)&light->isActive);

        const char* list[] = { "Directional", "Point", "Spot" };
        ImGui::Combo("Type", &light->type, list, 3);

        ImGui::ColorEdit3("Color", (float*)&light->color);
        ImGui::DragFloat3("Direction", (float*)&light->direction, 0.1f, -1, 1);       
        ImGui::DragFloat3("Position", (float*)&light->position, 1.0f, -100, 100);
        ImGui::SliderFloat("Range", (float*)&light->range, 1, 300);
        ImGui::SliderFloat("AttentionIntensity", (float*)&light->attentionIntensity, 0.1f, 3.0f);
        ImGui::SliderFloat("Inner", (float*)&light->inner, 1, 180);
        ImGui::SliderFloat("Outer", (float*)&light->outer, 1, 180);

        ImGui::TreePop();
    }
}
