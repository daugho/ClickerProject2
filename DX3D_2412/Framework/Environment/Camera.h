#pragma once

class Camera : public Transform
{
public:
    Camera();
    ~Camera();

    void Update();
    void Edit();

    void SetView(UINT slot = 1);

    class Ray ScreenPointToRay(const Vector3& screenPoint);

    Vector3 WorldToScreenPoint(Vector3 world);    

    void TargetOptionSave(string file);
    void TargetOptionLoad(string file);

    void SetTarget(Transform* target) { this->target = target; }

private:
    void FreeMode();
    void FollowMode();

private:
    ViewBuffer* viewBuffer;
    MatrixBuffer* projectionBuffer;

    Matrix view;
    Matrix projection;

    float moveSpeed = 5.0f;
    float rotSpeed = 10.0f;

    Vector3 prevMousePos;

    Transform* target = nullptr;

    float distance = 20.0f;
    float height = 20.0f;
    float moveDamping = 5.0f;
    float rotDamping = 1.0f;
    float destRot = 0.0f;

    Vector3 destPos;
    Vector3 focusOffset;

    Matrix rotMatrix;

    bool isLookAtTargetX = true;
    bool isLookAtTargetY = true;

    char file[128];
};