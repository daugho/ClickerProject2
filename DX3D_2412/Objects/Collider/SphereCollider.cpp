#include "Framework.h"

SphereCollider::SphereCollider(float radius, UINT stackCount, UINT sliceCount)
    : radius(radius), stackCount(stackCount), sliceCount(sliceCount)
{
    type = SPHERE;

    MakeMesh();
    mesh->CreateMesh();
}

bool SphereCollider::IsRayCollision(IN const Ray& ray, OUT RaycastHit* hit)
{
    if (!IsActive()) return false;

    Vector3 P = ray.origin;
    Vector3 D = ray.direction;

    Vector3 C = Center();
    Vector3 A = P - C;

    float b = Vector3::Dot(A, D);
    float c = Vector3::Dot(A, A) - pow(Radius(), 2);

    if (b * b >= c)
    {
        if (hit != nullptr)
        {
            float t = -b - sqrt(pow(b, 2) - c);

            hit->distance = t;
            hit->point = P + D * t;
            hit->normal = (hit->point - C).GetNormalized();
        }

        return true;
    }

    return false;

    return false;
}

bool SphereCollider::IsBoxCollision(BoxCollider* collider)
{
    return false;
}

bool SphereCollider::IsSphereCollision(SphereCollider* collider)
{
    float distance = Vector3::Distance(GetGlobalPosition(), collider->GetGlobalPosition());

    return distance <= Radius() + collider->Radius();
}

bool SphereCollider::IsCapsuleCollision(CapsuleCollider* collider)
{
    return false;
}

void SphereCollider::MakeMesh()
{
    float thetaStep = XM_2PI / sliceCount;
    float phiStep = XM_PI / stackCount;

    vector<Vertex>& vertices = mesh->GetVertices();

    vertices.reserve((sliceCount + 1) * (stackCount + 1));

    for (UINT i = 0; i <= stackCount; i++)
    {
        float phi = i * phiStep;

        for (int j = 0; j <= sliceCount; j++)
        {
            float theta = j * thetaStep;

            Vertex vertex;

            vertex.pos.x = sin(phi) * cos(theta) * radius;
            vertex.pos.y = cos(phi) * radius;
            vertex.pos.z = sin(phi) * sin(theta) * radius;

            vertices.push_back(vertex);
        }
    }

    vector<UINT>& indices = mesh->GetIndices();

    indices.reserve(sliceCount * stackCount * 4);

    for (UINT z = 0; z < stackCount; z++)
    {
        for (UINT x = 0; x < sliceCount; x++)
        {
            indices.push_back((sliceCount + 1) * z + x);//0            
            indices.push_back((sliceCount + 1) * (z + 1) + x);//1

            indices.push_back((sliceCount + 1) * z + x);//0                  
            indices.push_back((sliceCount + 1) * z + x + 1);//3            
        }
    }
}

bool SphereCollider::PushBox(BoxCollider* collider, RaycastHit* hit)
{
    return false;
}

bool SphereCollider::PushSphere(SphereCollider* collider, RaycastHit* hit)
{
    return false;
}

bool SphereCollider::PushCapsule(CapsuleCollider* collider, RaycastHit* hit)
{
    return false;
}
