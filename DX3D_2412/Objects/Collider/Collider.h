#pragma once

struct Ray
{
    Vector3 origin;
    Vector3 direction = { 0, 0, 1 };

    Ray() = default;
    Ray(Vector3 origin, Vector3 direction)
        : origin(origin), direction(direction)
    {}
};

struct RaycastHit
{
    Vector3 point;
    Vector3 normal;
    float distance;
};

class Collider : public GameObject<Vertex>
{
protected:
    enum Type
    {
        BOX, SPHERE, CAPSULE, RECT
    };

    const float PUSH_SPEED = 100.0f;

public:
    Collider();
    ~Collider() = default;

    void Render();

    bool IsCollision(Collider* collider);

    virtual bool IsRayCollision(IN const Ray& ray, OUT RaycastHit* hit = nullptr) = 0;
    virtual bool IsBoxCollision(class BoxCollider* collider) = 0;
    virtual bool IsSphereCollision(class SphereCollider* collider) = 0;
    virtual bool IsCapsuleCollision(class CapsuleCollider* collider) = 0;
    virtual void MakeMesh() = 0;

    virtual bool Push(Collider* collider, RaycastHit* hit = nullptr);
    virtual bool PushBox(class BoxCollider* collider, RaycastHit* hit = nullptr) = 0;
    virtual bool PushSphere(class SphereCollider* collider, RaycastHit* hit = nullptr) = 0;
    virtual bool PushCapsule(class CapsuleCollider* collider, RaycastHit* hit = nullptr) = 0;

    void SetColor(Float4 color) { material->GetData()->diffuse = color; }
    void SetColor(float r, float g, float b)
    {
        material->GetData()->diffuse = { r, g, b, 1 };
    }

    Vector3 Center()
    {
        return XMVector3TransformCoord(offset, world);        
    }

public:
    static void OnDraw() { isDraw = !isDraw; }

protected:
    Type type;
    
    Vector3 offset;

    static bool isDraw;
};