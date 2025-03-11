#include "Framework.h"

ModelExporter::ModelExporter(string name, string file) : name(name)
{
    importer = new Assimp::Importer();
        
    importer->SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);
    importer->SetPropertyInteger(AI_CONFIG_PP_RVC_FLAGS, aiComponent_TANGENTS_AND_BITANGENTS);

    scene = importer->ReadFile(file,
        aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_MaxQuality);

    assert(scene != nullptr);
}

ModelExporter::~ModelExporter()
{
    delete importer;
}

void ModelExporter::ExportMaterial()
{
    ReadMaterial();
    WriteMaterial();
}

void ModelExporter::ExportMesh()
{
    ReadNode(scene->mRootNode, -1, -1);
    ReadMesh(scene->mRootNode);
    WriteMesh();
}

void ModelExporter::ExportClip(string clipName)
{
    for (UINT i = 0; i < scene->mNumAnimations; i++)
    {
        Clip* clip = ReadClip(scene->mAnimations[i]);
        WriteClip(clip, clipName, i);
    }
}

void ModelExporter::ReadMaterial()
{
    for (int i = 0; i < scene->mNumMaterials; i++)
    {
        aiMaterial* srcMaterial = scene->mMaterials[i];
        Material* material = new Material();
        material->SetName(srcMaterial->GetName().C_Str());

        MaterialBuffer::Data* data = material->GetData();

        aiColor3D color;
        srcMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color);
        data->diffuse = Float4(color.r, color.g, color.b, 1.0f);

        srcMaterial->Get(AI_MATKEY_COLOR_SPECULAR, color);
        data->specular = Float4(color.r, color.g, color.b, 1.0f);

        srcMaterial->Get(AI_MATKEY_COLOR_AMBIENT, color);
        data->ambient = Float4(color.r, color.g, color.b, 1.0f);

        srcMaterial->Get(AI_MATKEY_COLOR_EMISSIVE, color);
        data->emissive = Float4(color.r, color.g, color.b, 1.0f);

        srcMaterial->Get(AI_MATKEY_SHININESS, data->shininess);

        aiString file;
        srcMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &file);
        material->SetDiffuseMap(CreateTexture(file.C_Str()));
        file.Clear();

        srcMaterial->GetTexture(aiTextureType_SPECULAR, 0, &file);
        material->SetSpecularMap(CreateTexture(file.C_Str()));
        file.Clear();

        srcMaterial->GetTexture(aiTextureType_NORMALS, 0, &file);
        material->SetNormalMap(CreateTexture(file.C_Str()));
        file.Clear();

        materials.push_back(material);
    }
}

void ModelExporter::WriteMaterial()
{
    string savePath = "Resources/Models/Materials/" + name + "/";
    string file = name + ".mats";

    Utility::CreateFolders(Utility::ToWString(savePath));

    BinaryWriter* writer = new BinaryWriter(savePath + file);

    writer->Data<UINT>(materials.size());

    for (Material* material : materials)
    {
        string path = savePath + material->GetName() + ".mat";
        material->Save(path);

        writer->String(path);

        delete material;
    }

    materials.clear();

    delete writer;
}

wstring ModelExporter::CreateTexture(string file)
{
    if (file.length() == 0)
        return L"";

    wstring fileName = Utility::GetFileNameWithoutExtension(file) + L".png";
    wstring path = L"Resources/Textures/Model/" +
        Utility::ToWString(name) + L"/" + fileName;

    Utility::CreateFolders(path);

    const aiTexture* srcTexture = scene->GetEmbeddedTexture(file.c_str());

    if (srcTexture == nullptr)
        return L"";

    if (srcTexture->mHeight < 1)
    {
        BinaryWriter w(path);
        w.Byte(srcTexture->pcData, srcTexture->mWidth);
    }
    else
    {
        Image image;
        image.width = srcTexture->mWidth;
        image.height = srcTexture->mHeight;
        image.pixels = (uint8_t*)(srcTexture->pcData);
        image.rowPitch = image.width * 4;
        image.slicePitch = image.width * image.height * 4;

        SaveToWICFile(image, WIC_FLAGS_NONE, GetWICCodec(WIC_CODEC_PNG),
            path.c_str());
    }

    return path;
}

void ModelExporter::ReadMesh(aiNode* node)
{
    for (UINT i = 0; i < node->mNumMeshes; i++)
    {
        MeshData* mesh = new MeshData();
        mesh->name = node->mName.C_Str();

        UINT index = node->mMeshes[i];
        aiMesh* srcMesh = scene->mMeshes[index];

        mesh->materialIndex = srcMesh->mMaterialIndex;

        vector<VertexWeights> vertexWeights(srcMesh->mNumVertices);
        ReadBone(srcMesh, vertexWeights);

        mesh->vertices.resize(srcMesh->mNumVertices);
        for (UINT v = 0; v < srcMesh->mNumVertices; v++)
        {
            ModelVertex vertex;
            memcpy(&vertex.pos, &srcMesh->mVertices[v], sizeof(Float3));

            if (srcMesh->HasTextureCoords(0))
                memcpy(&vertex.uv, &srcMesh->mTextureCoords[0][v], sizeof(Float2));

            if (srcMesh->HasNormals())
                memcpy(&vertex.normal, &srcMesh->mNormals[v], sizeof(Float3));

            if (srcMesh->HasTangentsAndBitangents())
                memcpy(&vertex.tangent, &srcMesh->mTangents[v], sizeof(Float3));

            if (!vertexWeights.empty())
            {
                vertexWeights[v].Normalize();

                vertex.indices.x = (float)vertexWeights[v].indices[0];
                vertex.indices.y = (float)vertexWeights[v].indices[1];
                vertex.indices.z = (float)vertexWeights[v].indices[2];
                vertex.indices.w = (float)vertexWeights[v].indices[3];

                vertex.weights.x = vertexWeights[v].weights[0];
                vertex.weights.y = vertexWeights[v].weights[1];
                vertex.weights.z = vertexWeights[v].weights[2];
                vertex.weights.w = vertexWeights[v].weights[3];
            }

            mesh->vertices[v] = vertex;
        }

        mesh->indices.resize(srcMesh->mNumFaces * 3);
        for (UINT f = 0; f < srcMesh->mNumFaces; f++)
        {
            aiFace& face = srcMesh->mFaces[f];

            for (UINT j = 0; j < face.mNumIndices; j++)
            {
                mesh->indices[f * 3 + j] = face.mIndices[j];
            }
        }

        meshes.push_back(mesh);
    }

    for (UINT i = 0; i < node->mNumChildren; i++)
        ReadMesh(node->mChildren[i]);
}

void ModelExporter::ReadNode(aiNode* node, int index, int parent)
{
    NodeData* nodeData = new NodeData();
    nodeData->index = index;
    nodeData->parent = parent;
    nodeData->name = node->mName.C_Str();

    Matrix matrix(node->mTransformation[0]);
    nodeData->transform = XMMatrixTranspose(matrix);

    nodes.push_back(nodeData);

    for (UINT i = 0; i < node->mNumChildren; i++)
        ReadNode(node->mChildren[i], nodes.size(), index);
}

void ModelExporter::ReadBone(aiMesh* mesh, vector<VertexWeights>& vertexWeights)
{
    for (int i = 0; i < mesh->mNumBones; i++)
    {
        UINT boneIndex = 0;
        string name = mesh->mBones[i]->mName.C_Str();

        if (boneMap.count(name) == 0)
        {
            boneIndex = boneCount++;
            boneMap[name] = boneIndex;

            BoneData* boneData = new BoneData();
            boneData->name = name;
            boneData->index = boneIndex;

            Matrix matrix(mesh->mBones[i]->mOffsetMatrix[0]);
            boneData->offset = XMMatrixTranspose(matrix);

            bones.push_back(boneData);
        }
        else
        {
            boneIndex = boneMap[name];
        }

        for (UINT j = 0; j < mesh->mBones[i]->mNumWeights; j++)
        {
            UINT index = mesh->mBones[i]->mWeights[j].mVertexId;
            vertexWeights[index].Add(boneIndex, mesh->mBones[i]->mWeights[j].mWeight);
        }
    }
}

void ModelExporter::WriteMesh()
{
    string path = "Resources/Models/Meshes/" + name + ".mesh";

    Utility::CreateFolders(path);

    BinaryWriter* writer = new BinaryWriter(path);

    writer->Data<UINT>(meshes.size());
    for (MeshData* mesh : meshes)
    {
        writer->String(mesh->name);
        writer->Data<UINT>(mesh->materialIndex);

        writer->Data<UINT>(mesh->vertices.size());
        writer->Byte(mesh->vertices.data(), sizeof(ModelVertex) * mesh->vertices.size());

        writer->Data<UINT>(mesh->indices.size());
        writer->Byte(mesh->indices.data(), sizeof(UINT) * mesh->indices.size());

        delete mesh;
    }
    meshes.clear();
        
    writer->Data<UINT>(nodes.size());
    for (NodeData* node : nodes)
    {
        writer->Data<int>(node->index);
        writer->String(node->name);
        writer->Data<int>(node->parent);
        writer->Data<Matrix>(node->transform);

        delete node;
    }
    nodes.clear();

    writer->Data<UINT>(bones.size());
    for (BoneData* bone : bones)
    {
        writer->Data<int>(bone->index);
        writer->String(bone->name);
        writer->Data<Matrix>(bone->offset);

        delete bone;
    }
    bones.clear();
    
    delete writer;
}

Clip* ModelExporter::ReadClip(aiAnimation* animation)
{
    Clip* clip = new Clip();
    clip->name = animation->mName.C_Str();
    clip->tickPerSecond = (float)animation->mTicksPerSecond;
    clip->frameCount = (UINT)(animation->mDuration) + 1;

    vector<ClipNode> clipNodes;
    clipNodes.reserve(animation->mNumChannels);
    for (UINT i = 0; i < animation->mNumChannels; i++)
    {
        aiNodeAnim* srcNode = animation->mChannels[i];

        ClipNode node;
        node.name = srcNode->mNodeName;

        KeyTransform transform;

        for (UINT k = 0; k < clip->frameCount; k++)
        {
            bool isFound = false;
            float t = node.transforms.size();

            if (k < srcNode->mNumPositionKeys &&
                GameMath::NearlyEqual(srcNode->mPositionKeys[k].mTime, t))
            {
                aiVectorKey key = srcNode->mPositionKeys[k];
                memcpy_s(&transform.pos, sizeof(Float3),
                    &key.mValue, sizeof(aiVector3D));

                isFound = true;
            }

            if (k < srcNode->mNumRotationKeys &&
                GameMath::NearlyEqual(srcNode->mRotationKeys[k].mTime, t))
            {
                aiQuatKey key = srcNode->mRotationKeys[k];

                transform.rot.x = (float)key.mValue.x;
                transform.rot.y = (float)key.mValue.y;
                transform.rot.z = (float)key.mValue.z;
                transform.rot.w = (float)key.mValue.w;

                isFound = true;
            }

            if (k < srcNode->mNumScalingKeys &&
                GameMath::NearlyEqual(srcNode->mScalingKeys[k].mTime, t))
            {
                aiVectorKey key = srcNode->mScalingKeys[k];
                memcpy_s(&transform.scale, sizeof(Float3),
                    &key.mValue, sizeof(aiVector3D));

                isFound = true;
            }

            if (isFound)
                node.transforms.push_back(transform);
        }

        if (node.transforms.size() < clip->frameCount)
        {
            UINT count = clip->frameCount - node.transforms.size();
            KeyTransform keyTransform = node.transforms.back();

            for (UINT n = 0; n < count; n++)
            {
                node.transforms.push_back(keyTransform);
            }
        }

        clipNodes.push_back(node);
    }

    ReadKeyFrame(clip, scene->mRootNode, clipNodes);

    return clip;
}

void ModelExporter::WriteClip(Clip* clip, string clipName, UINT index)
{
    string file = "Resources/Models/Clips/" + name + "/" + clipName + to_string(index) + ".clip";

    Utility::CreateFolders(file);

    BinaryWriter* writer = new BinaryWriter(file);
    writer->String(clip->name);
    writer->Data<UINT>(clip->frameCount);
    writer->Data<float>(clip->tickPerSecond);

    writer->Data<UINT>(clip->keyFrames.size());
    for (KeyFrame* keyFrame : clip->keyFrames)
    {
        writer->String(keyFrame->boneName);
        writer->Data<UINT>(keyFrame->transforms.size());
        writer->Byte(keyFrame->transforms.data(), sizeof(KeyTransform) * keyFrame->transforms.size());

        delete keyFrame;
    }

    delete clip;
    delete writer;
}

void ModelExporter::ReadKeyFrame(Clip* clip, aiNode* node, vector<ClipNode>& clipNodes)
{
    KeyFrame* keyFrame = new KeyFrame();
    keyFrame->boneName = node->mName.C_Str();
    keyFrame->transforms.reserve(clip->frameCount);
    for (UINT i = 0; i < clip->frameCount; i++)
    {
        ClipNode* clipNode = nullptr;
        for (ClipNode& temp : clipNodes)
        {
            if (temp.name == node->mName)
            {
                clipNode = &temp;
                break;
            }
        }

        KeyTransform keyTransform;
        if (clipNode == nullptr)
        {
            Matrix transform(node->mTransformation[0]);
            transform = XMMatrixTranspose(transform);

            Vector3 S, R, T;
            XMMatrixDecompose(S.Get(), R.Get(), T.Get(), transform);
            keyTransform.scale = S;
            XMStoreFloat4(&keyTransform.rot, R);
            keyTransform.pos = T;
        }
        else
        {
            keyTransform = clipNode->transforms[i];
        }

        keyFrame->transforms.push_back(keyTransform);
    }

    clip->keyFrames.push_back(keyFrame);

    for (UINT i = 0; i < node->mNumChildren; i++)
        ReadKeyFrame(clip, node->mChildren[i], clipNodes);
}

void ModelExporter::SetClipNode(const KeyData& keyData, const UINT& frameCount, ClipNode& clipNode)
{
}

Float3 ModelExporter::CalcInterpolationVector(const vector<KeyVector>& keyData, UINT& count, int curFrame)
{
    return Float3();
}

Float4 ModelExporter::CalcInterpolationQuat(const vector<KeyQuat>& keyData, UINT& count, int curFrame)
{
    return Float4();
}
