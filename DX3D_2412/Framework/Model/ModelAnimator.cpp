#include "Framework.h"

ModelAnimator::ModelAnimator(string name) : Model(name)
{
    frameBuffer = new FrameBuffer();

    //SetShader(L"Model/Model.hlsl");    
}

ModelAnimator::~ModelAnimator()
{
    for (ModelClip* clip : clips)
        delete clip;

    delete frameBuffer;

    delete[] clipTransforms;
    delete[] nodeTransforms;

    texture->Release();
    srv->Release();
}

void ModelAnimator::Update()
{
    UpdateFrame(frameBuffer->GetData());
    UpdateWorld();
}

void ModelAnimator::Render()
{
    frameBuffer->SetVS(3);
    DC->VSSetShaderResources(0, 1, &srv);

    Model::Render();
}

void ModelAnimator::Edit()
{
    Model::Edit();

    ImGui::SliderInt("Clip", &frameBuffer->GetData()->cur.clip, 0,
        clips.size() - 1);
    ImGui::SliderInt("Frame", &frameBuffer->GetData()->cur.curFrame, 0,
        clips[frameBuffer->GetData()->cur.clip]->frameCount - 1);
}

void ModelAnimator::ReadClip(string clipName, UINT clipNum)
{
    string file = "Resources/Models/Clips/" + name + "/" + clipName + to_string(clipNum) + ".clip";

    BinaryReader* reader = new BinaryReader(file);

    ModelClip* clip = new ModelClip();
    clip->name = reader->String();
    clip->frameCount = reader->Data<UINT>();
    clip->tickPerSecond = reader->Data<float>();

    UINT boneCount = reader->Data<UINT>();
    for (UINT i = 0; i < boneCount; i++)
    {
        KeyFrame* keyFrame = new KeyFrame();

        keyFrame->boneName = reader->String();
        UINT size = reader->Data<UINT>();
        if (size > 0)
        {
            keyFrame->transforms.resize(size);

            void* ptr = (void*)keyFrame->transforms.data();
            reader->Byte(&ptr, sizeof(KeyTransform) * size);
        }
        clip->keyFrames[keyFrame->boneName] = keyFrame;
    }

    clips.push_back(clip);

    delete reader;
}

void ModelAnimator::PlayClip(int clip, float scale, float duration)
{
    isPlay = true;

    frameBuffer->GetData()->next.clip = clip;
    frameBuffer->GetData()->next.scale = scale;
    frameBuffer->GetData()->duration = duration;

    clips[clip]->Init();
}

void ModelAnimator::CreateTexture()
{
    UINT clipCount = clips.size();

    clipTransforms = new ClipTransform[clipCount];
    nodeTransforms = new ClipTransform[clipCount];

    FOR(clipCount)
        CreateClipTransform(i);

    UINT boneNum = nodes.size();
    UINT frameNum = GetMaxFrameNum();

    D3D11_TEXTURE2D_DESC desc = {};
    desc.Width = boneNum * 4;
    desc.Height = frameNum;
    desc.MipLevels = 1;
    desc.ArraySize = clipCount;
    desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_IMMUTABLE;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

    UINT pitchSize = boneNum * sizeof(Matrix);
    UINT pageSize = pitchSize * frameNum;

    void* p = VirtualAlloc(nullptr, pageSize * clipCount,
        MEM_RESERVE, PAGE_READWRITE);

    FOR(clipCount)
    {
        UINT start = i * pageSize;

        for (UINT y = 0; y < frameNum; y++)
        {
            void* temp = (BYTE*)p + pitchSize * y + start;

            VirtualAlloc(temp, pitchSize, MEM_COMMIT, PAGE_READWRITE);
            memcpy(temp, clipTransforms[i].transform[y], pitchSize);
        }
    }

    D3D11_SUBRESOURCE_DATA* subResource = new D3D11_SUBRESOURCE_DATA[clipCount];

    FOR(clipCount)
    {
        void* temp = (BYTE*)p + i * pageSize;

        subResource[i].pSysMem = temp;
        subResource[i].SysMemPitch = pitchSize;
        subResource[i].SysMemSlicePitch = pageSize;
    }

    DEVICE->CreateTexture2D(&desc, subResource, &texture);

    delete[] subResource;
    VirtualFree(p, 0, MEM_RELEASE);

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
    srvDesc.Texture2DArray.MipLevels = 1;
    srvDesc.Texture2DArray.ArraySize = clipCount;

    DEVICE->CreateShaderResourceView(texture, &srvDesc, &srv);
}

Matrix ModelAnimator::GetTransformByNode(int nodeIndex)
{
    return Matrix();
}

void ModelAnimator::CreateClipTransform(UINT index)
{
    ModelClip* clip = clips[index];

    FOR(clip->frameCount)
    {
        UINT nodeIndex = 0;

        for (NodeData node : nodes)
        {
            Matrix animation;
            KeyFrame* frame = clip->GetKeyFrame(node.name);
            if (frame != nullptr)
            {
                KeyTransform& transform = frame->transforms[i];

                animation = XMMatrixTransformation(XMVectorZero(),
                    XMQuaternionIdentity(), Vector3(transform.scale),
                    XMVectorZero(), XMLoadFloat4(&transform.rot),
                    Vector3(transform.pos));
            }
            else
            {
                animation = XMMatrixIdentity();
            }

            Matrix parent;
            int parentIndex = node.parent;

            if (parentIndex < 0)
                parent = XMMatrixIdentity();
            else
                parent = nodeTransforms[index].transform[i][parentIndex];

            nodeTransforms[index].transform[i][nodeIndex] = animation * parent;

            int boneIndex = -1;
            if (boneMap.count(node.name))
                boneIndex = boneMap[node.name];

            if (boneIndex >= 0)
            {
                Matrix offset = bones[boneIndex].offset * nodeTransforms[index].transform[i][nodeIndex];
                clipTransforms[index].transform[i][boneIndex] = offset;
            }

            nodeIndex++;
        }
    }
}

void ModelAnimator::UpdateFrame(Motion* motion)
{
    if (!isPlay) return;
    {
        Frame* frame = &motion->cur;
        ModelClip* clip = clips[frame->clip];

        frame->time += clip->tickPerSecond * frame->scale * DELTA;
        motion->runningTime += frame->scale * DELTA;
        clip->playTime += frame->scale * DELTA;

        if (frame->time >= 1.0f)
        {
            ++frame->curFrame %= clip->frameCount - 1;
            frame->time -= 1.0f;
        }
    }

    {
        Frame* frame = &motion->next;

        if (frame->clip < 0) return;

        ModelClip* clip = clips[frame->clip];

        motion->tweenTime += DELTA / motion->duration;

        if (motion->tweenTime >= 1.0f)
        {
            motion->cur = motion->next;
            motion->tweenTime = 0.0f;

            motion->next.clip = -1;
            motion->next.curFrame = 0;
            motion->next.time = 0.0f;
            return;
        }

        frame->time += clip->tickPerSecond * frame->scale * DELTA;

        if (frame->time >= 1.0f)
        {
            ++frame->curFrame %= clip->frameCount - 1;
            frame->time -= 1.0f;
        }
    }
}

UINT ModelAnimator::GetMaxFrameNum()
{
    UINT maxNum = 0;

    for (ModelClip* clip : clips)
    {
        if (clip->frameCount > maxNum)
            maxNum = clip->frameCount;
    }

    return maxNum;
}
