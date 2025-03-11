#include "Framework.h"

CapsuleCollider::CapsuleCollider(float radius, float height, UINT stackCount, UINT sliceCount)
    : radius(radius), height(height), stackCount(stackCount), sliceCount(sliceCount)
{
    type = CAPSULE;

    MakeMesh();
    mesh->CreateMesh();
}

void CapsuleCollider::Edit()
{
    Transform::Edit();

    if (ImGui::TreeNode((tag + "_Collider").c_str()))
    {
        string key = tag + "_Radius";
        if (ImGui::DragFloat(key.c_str(), &radius, 0.1f))
        {
            UpdateMesh();
        }

        key = tag + "_Height";
        if (ImGui::DragFloat(key.c_str(), &height, 0.1f))
        {
            UpdateMesh();
        }

        key = tag + "_Offset";
        if (ImGui::DragFloat3(key.c_str(), (float*)&offset, 0.1f))
        {
            UpdateMesh();
        }

        if (ImGui::Button("Save"))
            CapsuleSave();

        ImGui::SameLine();

        if (ImGui::Button("Load"))
            CapsuleLoad();

        ImGui::TreePop();
    }
}

bool CapsuleCollider::IsRayCollision(IN const Ray& ray, OUT RaycastHit* hit)
{
    if (!IsActive()) return false;

    Vector3 direction = GetUp();
    Vector3 pa = Center() - direction * Height() * 0.5f;
    Vector3 pb = Center() + direction * Height() * 0.5f;

    float r = Radius();

    Vector3 ro = ray.origin;
    Vector3 rd = ray.direction;

    Vector3 ba = pb - pa;
    Vector3 oa = ro - pa;

    float baba = Vector3::Dot(ba, ba);
    float bard = Vector3::Dot(ba, rd);
    float baoa = Vector3::Dot(ba, oa);
    float rdoa = Vector3::Dot(rd, oa);
    float oaoa = Vector3::Dot(oa, oa);

    float a = baba - bard * bard;
    float b = baba * rdoa - baoa * bard;
    float c = baba * oaoa - baoa * baoa - r * r * baba;

    float h = b * b - a * c;

    if (h >= 0.0f)
    {
        float t = (-b - sqrt(h)) / a;

        float distance = baoa + t * bard;

        if (distance > 0.0f && distance < baba)
        {
            if (hit)
            {
                hit->distance = distance;
                hit->point = ray.origin + ray.direction * t;
            }

            return true;
        }

        Vector3 oc = (distance <= 0.0f) ? oa : ro - pb;
        b = Vector3::Dot(rd, oc);
        c = Vector3::Dot(oc, oc) - r * r;
        h = b * b - c;

        if (h > 0.0f)
        {
            if (hit)
            {
                hit->distance = distance;
                hit->point = ray.origin + ray.direction * t;
            }

            return true;
        }
    }

    return false;
}

bool CapsuleCollider::IsBoxCollision(BoxCollider* collider)
{
    return false;
}

bool CapsuleCollider::IsSphereCollision(SphereCollider* collider)
{
    return false;
}

bool CapsuleCollider::IsCapsuleCollision(CapsuleCollider* collider)
{
    Vector3 aDirection = GetUp();
    Vector3 aA = GetGlobalPosition() - aDirection * Height() * 0.5f;
    Vector3 aB = GetGlobalPosition() + aDirection * Height() * 0.5f;

    Vector3 bDirection = collider->GetUp();
    Vector3 bA = collider->GetGlobalPosition() - bDirection * collider->Height() * 0.5f;
    Vector3 bB = collider->GetGlobalPosition() + bDirection * collider->Height() * 0.5f;

    Vector3 v0 = bA - aA;
    Vector3 v1 = bB - aA;
    Vector3 v2 = bA - aB;
    Vector3 v3 = bB - aB;

    float d0 = Vector3::Dot(v0, v0);
    float d1 = Vector3::Dot(v1, v1);
    float d2 = Vector3::Dot(v2, v2);
    float d3 = Vector3::Dot(v3, v3);

    Vector3 bestA;
    if (d2 < d0 || d2 < d1 || d3 < d0 || d3 > d1)
        bestA = aB;
    else
        bestA = aA;

    Vector3 bestB = GameMath::ClosestPointOnLine(bA, bB, bestA);
    bestA = GameMath::ClosestPointOnLine(aA, aB, bestB);
    bestB = GameMath::ClosestPointOnLine(bA, bB, bestA);

    float distance = Vector3::Distance(bestA, bestB);

    return distance <= (Radius() + collider->Radius());
}

void CapsuleCollider::CapsuleSave()
{
    BinaryWriter* writer = new BinaryWriter("Resources/Colliders/" + tag + ".col");
    writer->Data<float>(radius);
    writer->Data<float>(height);
    writer->Data<Vector3>(offset);

    delete writer;
}

void CapsuleCollider::CapsuleLoad()
{
    BinaryReader* reader = new BinaryReader("Resources/Colliders/" + tag + ".col");

    if (reader->IsFailed())
    {
        delete reader;
        return;
    }

    radius = reader->Data<float>();
    height = reader->Data<float>();
    offset = reader->Data<Vector3>();
    UpdateMesh();
}

void CapsuleCollider::MakeMesh()
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

            if (vertex.pos.y > 0.0f)
                vertex.pos.y += height * 0.5f;
            else if (vertex.pos.y < 0.0f)
                vertex.pos.y -= height * 0.5f;

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

void CapsuleCollider::UpdateMesh()
{
    float thetaStep = XM_2PI / sliceCount;
    float phiStep = XM_PI / stackCount;

    vector<Vertex>& vertices = mesh->GetVertices();

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

            if (vertex.pos.y > 0.0f)
                vertex.pos.y += height * 0.5f;
            else if (vertex.pos.y < 0.0f)
                vertex.pos.y -= height * 0.5f;

            vertex.pos += offset;

            int index = (sliceCount + 1) * i + j;
            vertices[index] = vertex;
        }
    }

    mesh->UpdateVertices();
}

bool CapsuleCollider::PushBox(BoxCollider* collider, RaycastHit* hit)
{
    return false;
}

bool CapsuleCollider::PushSphere(SphereCollider* collider, RaycastHit* hit)
{
    return false;
}

bool CapsuleCollider::PushCapsule(CapsuleCollider* collider, RaycastHit* hit)
{
    return false;
}
