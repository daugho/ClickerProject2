#pragma once

class CapsuleCollider : public Collider
{
public:
    CapsuleCollider(float radius = 1.0f, float height = 2.0f,
        UINT stackCount = 9, UINT sliceCount = 16);

    virtual void Edit() override;

    virtual bool IsRayCollision(IN const Ray& ray, OUT RaycastHit* hit) override;
    virtual bool IsBoxCollision(BoxCollider* collider) override;
    virtual bool IsSphereCollision(SphereCollider* collider) override;
    virtual bool IsCapsuleCollision(CapsuleCollider* collider) override;
    
    virtual bool PushBox(BoxCollider* collider, RaycastHit* hit) override;
    virtual bool PushSphere(SphereCollider* collider, RaycastHit* hit) override;
    virtual bool PushCapsule(CapsuleCollider* collider, RaycastHit* hit) override;

    float Radius() {
        return radius * max(GetGlobalScale().x,
            max(GetGlobalScale().y, GetGlobalScale().z));
    }

    float Height() { return height * GetGlobalScale().y; }

    void CapsuleSave();
    void CapsuleLoad();

private:
    virtual void MakeMesh() override;
    void UpdateMesh();

private:
    float radius, height;
    UINT stackCount, sliceCount;
};