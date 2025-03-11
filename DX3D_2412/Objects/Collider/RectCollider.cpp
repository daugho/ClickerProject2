#include "Framework.h"

RectCollider::RectCollider()
{
    type = Type::RECT;
}

RectCollider::RectCollider(Vector2 size)
    : size(size)
{
    type = Type::RECT;

    MakeMesh();
    mesh->CreateMesh();    
}

bool RectCollider::IsPointCollision(Vector3 point)
{
    Matrix invWorld = XMMatrixInverse(nullptr, world);

    point *= invWorld;

    Vector2 half = size * 0.5f;

    return abs(point.x) < half.x && abs(point.y) < half.y;
}

void RectCollider::CreateMesh(Vector2 size)
{
    this->size = size;

    MakeMesh();
    mesh->CreateMesh();
}

/*
bool RectCollider::IsPointCollision(Vector2 point)
{
    ObbDesc obb = GetObb();

    Vector2 direction = obb.position - point;

    float d = abs(Dot(direction, obb.axis[0]));

    if (d > obb.halfSize.x) return false;

    d = abs(Dot(direction, obb.axis[1]));

    if (d > obb.halfSize.y) return false;

    return true;
}
*/

Vector2 RectCollider::LeftTop()
{
    Vector2 edge = Vector2(-size.x, +size.y) * 0.5f;

    return edge * world;
}

Vector2 RectCollider::LeftBottom()
{
    Vector2 edge = Vector2(-size.x, -size.y) * 0.5f;

    return edge * world;
}

Vector2 RectCollider::RightTop()
{
    Vector2 edge = Vector2(+size.x, +size.y) * 0.5f;

    return edge * world;
}

Vector2 RectCollider::RightBottom()
{
    Vector2 edge = Vector2(+size.x, -size.y) * 0.5f;

    return edge * world;
}

float RectCollider::L()
{
    float minLeft = min(LeftTop().x, LeftBottom().x);
    float minRight = min(RightTop().x, RightBottom().x);

    return min(minLeft, minRight);
}

float RectCollider::R()
{
    float maxLeft = max(LeftTop().x, LeftBottom().x);
    float maxRight = max(RightTop().x, RightBottom().x);

    return max(maxLeft, maxRight);
}

float RectCollider::T()
{
    float maxTop = max(LeftTop().y, RightTop().y);
    float maxBottom = max(LeftBottom().y, RightBottom().y);

    return max(maxTop, maxBottom);
}

float RectCollider::B()
{
    float minTop = min(LeftTop().y, RightTop().y);
    float minBottom = min(LeftBottom().y, RightBottom().y);

    return min(minTop, minBottom);
}

bool RectCollider::IsRayCollision(IN const Ray& ray, OUT RaycastHit* hit)
{
    return false;
}

bool RectCollider::IsBoxCollision(BoxCollider* collider)
{
    return false;
}

bool RectCollider::IsSphereCollision(SphereCollider* collider)
{
    return false;
}

bool RectCollider::IsCapsuleCollision(CapsuleCollider* collider)
{
    return false;
}

void RectCollider::MakeMesh()
{
    Vector2 halfSize = size * 0.5f;

    vector<Vertex>& vertices = mesh->GetVertices();

    vertices.emplace_back(-halfSize.x, +halfSize.y);
    vertices.emplace_back(+halfSize.x, +halfSize.y);
    vertices.emplace_back(+halfSize.x, -halfSize.y);
    vertices.emplace_back(-halfSize.x, -halfSize.y);
    vertices.emplace_back(-halfSize.x, +halfSize.y);

    vector<UINT>& indices = mesh->GetIndices();
    indices = { 0, 1, 1, 2, 2, 3, 3, 4 };
}

bool RectCollider::PushBox(BoxCollider* collider, RaycastHit* hit)
{
    return false;
}

bool RectCollider::PushSphere(SphereCollider* collider, RaycastHit* hit)
{
    return false;
}

bool RectCollider::PushCapsule(CapsuleCollider* collider, RaycastHit* hit)
{
    return false;
}