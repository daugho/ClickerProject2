#include "Framework.h"

Transform::Transform()
{
	world = XMMatrixIdentity();
}

void Transform::Edit()
{
    if (ImGui::TreeNode((tag + "_Transform").c_str()))
    {
        ImGui::Checkbox("Active", &isActive);

        ImGui::DragFloat3("Pos", (float*)&localPosition, 0.1f);

        Float3 rot;
        rot.x = XMConvertToDegrees(localRotation.x);
        rot.y = XMConvertToDegrees(localRotation.y);
        rot.z = XMConvertToDegrees(localRotation.z);

        if (ImGui::DragFloat3("Rot", (float*)&rot, 0.1f, -180, 180))
        {
            localRotation.x = XMConvertToRadians(rot.x);
            localRotation.y = XMConvertToRadians(rot.y);
            localRotation.z = XMConvertToRadians(rot.z);
        }

        ImGui::DragFloat3("Scale", (float*)&localScale, 0.1f);
        ImGui::DragFloat3("Pivot", (float*)&pivot, 0.1f);

        if (ImGui::Button("Save"))
            Save();
        if (ImGui::Button("Load"))
            Load();

        ImGui::TreePop();
    }
}

void Transform::Save()
{
    BinaryWriter* writer = new BinaryWriter("Resources/Transforms/" + tag + ".srt");

    writer->Data<bool>(isActive);
    writer->Data<Vector3>(localPosition);
    writer->Data<Vector3>(localRotation);
    writer->Data<Vector3>(localScale);
    writer->Data<Vector3>(pivot);

    delete writer;
}

void Transform::Load()
{
    BinaryReader* reader = new BinaryReader("Resources/Transforms/" + tag + ".srt");

    if (reader->IsFailed())
    {
        delete reader;
        return;
    }

    isActive = reader->Data<bool>();
    localPosition = reader->Data<Vector3>();
    localRotation = reader->Data<Vector3>();
    localScale = reader->Data<Vector3>();
    pivot = reader->Data<Vector3>();

    delete reader;

    UpdateWorld();
}

void Transform::UpdateWorld()
{
	//Matrix S = XMMatrixScaling(localScale.x, localScale.y, localScale.z);
	//Matrix R = XMMatrixRotationRollPitchYaw(localRotation.x, localRotation.y, localRotation.z);
	//Matrix T = XMMatrixTranslation(localPosition.x, localPosition.y, localPosition.z);
	//
	//Matrix P = XMMatrixTranslation(pivot.x, pivot.y, pivot.z);
	//Matrix IP = XMMatrixInverse(nullptr, P);
	//
	//world = IP * S * R * T * P;

	world = XMMatrixTransformation(pivot, XMQuaternionIdentity(),
		localScale, pivot, XMQuaternionRotationRollPitchYawFromVector(localRotation),
		localPosition);

	if (parent)
		world *= parent->world;

	XMStoreFloat4x4(&matWorld, world);
	right = Vector3(matWorld._11, matWorld._12, matWorld._13);
	up = Vector3(matWorld._21, matWorld._22, matWorld._23);
	forward = Vector3(matWorld._31, matWorld._32, matWorld._33);

	XMMatrixDecompose(globalScale.Get(), globalRotation.Get(), globalPosition.Get(), world);
}

void Transform::SetGlobalPosition(const Vector3& position)
{
    localPosition = position;

    if (parent)
        localPosition -= parent->GetGlobalPosition();
}

bool Transform::IsActive()
{
    if (parent == nullptr)
        return isActive;

    if (isActive == false)
        return false;

    return parent->IsActive();
}