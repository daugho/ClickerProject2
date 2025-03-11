#pragma once

class RectCollider : public Collider
{
public:
    RectCollider();
    RectCollider(Vector2 size);
    ~RectCollider() = default;
    
    virtual bool IsRayCollision(IN const Ray& ray, OUT RaycastHit* hit) override;
    virtual bool IsBoxCollision(BoxCollider* collider) override;
    virtual bool IsSphereCollision(SphereCollider* collider) override;
    virtual bool IsCapsuleCollision(CapsuleCollider* collider) override;
    
    virtual bool PushBox(BoxCollider* collider, RaycastHit* hit) override;
    virtual bool PushSphere(SphereCollider* collider, RaycastHit* hit) override;
    virtual bool PushCapsule(CapsuleCollider* collider, RaycastHit* hit) override;

    bool IsPointCollision(Vector3 point);

    void CreateMesh(Vector2 size);

    Vector2 Size() { return size; }
    Vector2 Half() { return Size() * 0.5f; }

    Vector2 LeftTop();
    Vector2 LeftBottom();
    Vector2 RightTop();
    Vector2 RightBottom();

    float L();
    float R();
    float T();
    float B();

private:
    virtual void MakeMesh() override;

protected:
    Vector2 size;       
};