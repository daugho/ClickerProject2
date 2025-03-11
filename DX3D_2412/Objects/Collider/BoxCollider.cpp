#include "Framework.h"

BoxCollider::BoxCollider(Vector3 size) : size(size)
{
    type = BOX;

    MakeMesh();
    mesh->CreateMesh();
}

bool BoxCollider::IsRayCollision(IN const Ray& ray, OUT RaycastHit* hit)
{
    if (!IsActive()) return false;

    ObbDesc box;
    GetObb(box);

    Vector3 min = box.halfSize * -1.0f;
    Vector3 max = box.halfSize;

    Vector3 delta = box.center - ray.origin;
    Vector3 D = ray.direction.GetNormalized();

    float tMin = 0.0f;
    float tMax = FLT_MAX;

    int hitAxis = -1;

    for (int i = 0; i < 3; i++)
    {
        Vector3 axis = box.axis[i];
        float e = Vector3::Dot(axis, delta);
        float f = Vector3::Dot(axis, D);

        if (GameMath::NearlyEqual(f, 0.0f))
        {
            if (min[i] > e || max[i] < e) return false;
        }
        else
        {
            float t1 = (e + min[i]) / f;
            float t2 = (e + max[i]) / f;

            if (t1 > t2) swap(t1, t2);

            if (t2 < tMax) tMax = t2;
            if (t1 > tMin)
            {
                tMin = t1;
                hitAxis = i;
            }

            if (tMin > tMax) return false;
        }
    }

    if (hit != nullptr)
    {
        hit->distance = tMin;
        hit->point = ray.origin + D * tMin;

        hit->normal = box.axis[hitAxis];
        if (Vector3::Dot(D, hit->normal) > 0)
            hit->normal *= -1;
    }

    return true;
}

bool BoxCollider::IsBoxCollision(BoxCollider* collider)
{
    ObbDesc box1, box2;
    GetObb(box1);
    collider->GetObb(box2);

    Vector3 D = box2.center - box1.center;

    for (int i = 0; i < 3; i++)
    {
        if (IsSeperateAxis(D, box1.axis[i], box1, box2)) return false;
        if (IsSeperateAxis(D, box2.axis[i], box1, box2)) return false;

        for (int j = 0; j < 3; j++)
        {
            Vector3 cross = Vector3::Cross(box1.axis[i], box2.axis[j]);
            if (IsSeperateAxis(D, cross, box1, box2)) return false;
        }
    }

    return true;
}

bool BoxCollider::IsSphereCollision(SphereCollider* collider)
{
    return false;
}

bool BoxCollider::IsCapsuleCollision(CapsuleCollider* collider)
{
    return false;
}

void BoxCollider::MakeMesh()
{
    vector<Vertex>& vertices = mesh->GetVertices();
    //Front
    vertices.emplace_back(-0.5f * size.x, +0.5f * size.y, -0.5f * size.z);//0
    vertices.emplace_back(+0.5f * size.x, +0.5f * size.y, -0.5f * size.z);//1
    vertices.emplace_back(-0.5f * size.x, -0.5f * size.y, -0.5f * size.z);//2
    vertices.emplace_back(+0.5f * size.x, -0.5f * size.y, -0.5f * size.z);//3
    //Back
    vertices.emplace_back(-0.5f * size.x, +0.5f * size.y, +0.5f * size.z);//4    
    vertices.emplace_back(+0.5f * size.x, +0.5f * size.y, +0.5f * size.z);//5
    vertices.emplace_back(-0.5f * size.x, -0.5f * size.y, +0.5f * size.z);//6
    vertices.emplace_back(+0.5f * size.x, -0.5f * size.y, +0.5f * size.z);//7

    vector<UINT>& indices = mesh->GetIndices();
    indices =
    {
        0, 1, 2, 3, 4, 5, 6, 7,
        0, 2, 1, 3, 4, 6, 5, 7,
        0, 4, 1, 5, 2, 6, 3, 7
    };
}

bool BoxCollider::IsSeperateAxis(const Vector3& D, const Vector3& axis, const ObbDesc& box1, const ObbDesc& box2)
{
    float distance = abs(Vector3::Dot(D, axis));

    float a = 0.0f;
    float b = 0.0f;

    for (int i = 0; i < 3; i++)
    {
        Vector3 temp = box1.axis[i] * box1.halfSize[i];
        a += abs(Vector3::Dot(temp, axis));
        temp = box2.axis[i] * box2.halfSize[i];
        b += abs(Vector3::Dot(temp, axis));
    }

    return distance > a + b;
}

bool BoxCollider::PushBox(BoxCollider* collider, RaycastHit* hit)
{
    return false;
}

bool BoxCollider::PushSphere(SphereCollider* collider, RaycastHit* hit)
{
    return false;
}

bool BoxCollider::PushCapsule(CapsuleCollider* collider, RaycastHit* hit)
{
    return false;
}

void BoxCollider::GetObb(ObbDesc& obbDesc)
{
    obbDesc.center = Center();

    obbDesc.axis[0] = GetRight();
    obbDesc.axis[1] = GetUp();
    obbDesc.axis[2] = GetForward();

    obbDesc.halfSize = size * 0.5f * GetGlobalScale();
}

void BoxCollider::UpdateMesh(const Vector3& size)
{
    this->size = size;

    vector<Vertex>& vertices = mesh->GetVertices();
    vertices.clear();
    //Front
    vertices.emplace_back(-0.5f * size.x, +0.5f * size.y, -0.5f * size.z);//0
    vertices.emplace_back(+0.5f * size.x, +0.5f * size.y, -0.5f * size.z);//1
    vertices.emplace_back(-0.5f * size.x, -0.5f * size.y, -0.5f * size.z);//2
    vertices.emplace_back(+0.5f * size.x, -0.5f * size.y, -0.5f * size.z);//3
    //Back
    vertices.emplace_back(-0.5f * size.x, +0.5f * size.y, +0.5f * size.z);//4    
    vertices.emplace_back(+0.5f * size.x, +0.5f * size.y, +0.5f * size.z);//5
    vertices.emplace_back(-0.5f * size.x, -0.5f * size.y, +0.5f * size.z);//6
    vertices.emplace_back(+0.5f * size.x, -0.5f * size.y, +0.5f * size.z);//7

    mesh->UpdateVertices();
}
