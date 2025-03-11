#include "Framework.h"
#include "Camera.h"

Camera::Camera()
{
	tag = "Camera";

    viewBuffer = new ViewBuffer();
    projectionBuffer = new MatrixBuffer();


    projection = XMMatrixPerspectiveFovLH(XM_PI / 3,
        (float)SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 1000.0f);
    projectionBuffer->Set(projection);    

    prevMousePos = mousePos;
}

Camera::~Camera()
{
}

void Camera::Update()
{
    if (target)
        FollowMode();
    else
        FreeMode();

    UpdateWorld();
}

void Camera::Edit()
{
    if (ImGui::TreeNode("Camera Option"))
    {
        Transform::Edit();

        ImGui::DragFloat("MoveSpeed", &moveSpeed);
        ImGui::DragFloat("RotSpeed", &rotSpeed);

        if (target)
        {
            ImGui::DragFloat("Distance", &distance);
            ImGui::DragFloat("Height", &height);

            ImGui::DragFloat("MoveDamping", &moveDamping, 0.1f);
            ImGui::DragFloat("RotDamping", &rotDamping, 0.1f);

            ImGui::DragFloat3("FocusOffset", (float*)&focusOffset, 0.1f);

            ImGui::Checkbox("LookAtTargetX", &isLookAtTargetX);
            ImGui::Checkbox("LookAtTargetY", &isLookAtTargetY);

            ImGui::InputText("File", file, 128);

            if (ImGui::Button("Save"))
                TargetOptionSave(file);

            ImGui::SameLine();

            if (ImGui::Button("Load"))
                TargetOptionLoad(file);
        }

        ImGui::TreePop();
    }
}

void Camera::SetView(UINT slot)
{
    view = XMMatrixInverse(nullptr, world);

    viewBuffer->Set(view, world);
    viewBuffer->SetVS(slot);   
    projectionBuffer->SetVS(slot + 1);
}

Ray Camera::ScreenPointToRay(const Vector3& screenPoint)
{
    Vector3 screenSize = Vector3(SCREEN_WIDTH, SCREEN_HEIGHT);

    Vector3 point = screenPoint / screenSize * 2.0f;
    point -= 1.0f;

    Float4x4 temp;
    XMStoreFloat4x4(&temp, projection);

    point.x /= temp._11;
    point.y /= temp._22;
    point.z = 1.0f;

    point = XMVector3TransformNormal(point, world);

    return Ray(GetGlobalPosition(), point.GetNormalized());
}

Vector3 Camera::WorldToScreenPoint(Vector3 world)
{
    Vector3 screenPos;

    screenPos = XMVector3TransformCoord(world, view);
    screenPos = XMVector3TransformCoord(screenPos, projection);
    //NDC : -1 ~ 1

    //Viewport º¯È¯
    screenPos = (screenPos + Vector3::One()) * 0.5f;//0~1

    screenPos.x *= SCREEN_WIDTH;
    screenPos.y *= SCREEN_HEIGHT;

    return screenPos;
}

void Camera::TargetOptionSave(string file)
{
    string path = "Resources/Camera/" + file + ".cam";

    BinaryWriter* writer = new BinaryWriter(path);

    writer->Data<float>(distance);
    writer->Data<float>(height);
    writer->Data<float>(moveDamping);
    writer->Data<float>(rotDamping);
    writer->Data<Vector3>(focusOffset);
    writer->Data<bool>(isLookAtTargetX);
    writer->Data<bool>(isLookAtTargetY);

    delete writer;
}

void Camera::TargetOptionLoad(string file)
{
    memcpy(this->file, file.c_str(), file.size());
    this->file[file.size()] = 0;

    string path = "Resources/Camera/" + file + ".cam";

    BinaryReader* reader = new BinaryReader(path);

    distance = reader->Data<float>();
    height = reader->Data<float>();
    moveDamping = reader->Data<float>();
    rotDamping = reader->Data<float>();
    focusOffset = reader->Data<Vector3>();
    isLookAtTargetX = reader->Data<bool>();
    isLookAtTargetY = reader->Data<bool>();

    delete reader;
}

void Camera::FreeMode()
{
    Vector3 delta = mousePos - prevMousePos;
    prevMousePos = mousePos;

    if (KEY->Press(VK_RBUTTON))
    {
        if (KEY->Press('W'))
            Translate(GetForward() * moveSpeed * DELTA);
        if (KEY->Press('S'))
            Translate(GetBack() * moveSpeed * DELTA);
        if (KEY->Press('A'))
            Translate(GetLeft() * moveSpeed * DELTA);
        if (KEY->Press('D'))
            Translate(GetRight() * moveSpeed * DELTA);
        if (KEY->Press('Q'))
            Translate(GetDown() * moveSpeed * DELTA);
        if (KEY->Press('E'))
            Translate(GetUp() * moveSpeed * DELTA);

        Rotate(Vector3::Up(), delta.x * rotSpeed * DELTA);
        Rotate(Vector3::Left(), delta.y * rotSpeed * DELTA);
    }
}

void Camera::FollowMode()
{
    destRot = GameMath::Lerp<float>(destRot, target->GetLocalRotation().y, rotDamping * DELTA);
    rotMatrix = XMMatrixRotationY(destRot);

    Vector3 forward = Vector3::Forward() * rotMatrix;

    destPos = target->GetGlobalPosition() - forward * distance;
    destPos.y += height;

    localPosition = GameMath::Lerp<Vector3>(
        localPosition, destPos, moveDamping * DELTA);

    Vector3 offset = focusOffset * rotMatrix;
    Vector3 targetPos = target->GetGlobalPosition() + offset;

    Vector3 dir = targetPos - localPosition;
    if (GameMath::NearlyEqual(dir.Magnitude(), 0.0f))
        return;

    dir.Normalize();
    forward = Vector3(dir.x, 0.0f, dir.z);
    if (GameMath::NearlyEqual(forward.Magnitude(), 0.0f))
        return;
    forward.Normalize();

    float dot = Vector3::Dot(dir, forward);
    dot = GameMath::Clamp(-1.0f, 1.0f, dot);

    if (isLookAtTargetX)
        localRotation.x = acos(dot);
    if (isLookAtTargetY)
        localRotation.y = atan2(dir.x, dir.z);
}
