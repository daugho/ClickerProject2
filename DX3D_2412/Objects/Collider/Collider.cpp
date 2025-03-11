#include "Framework.h"

bool Collider::isDraw = true;

Collider::Collider() : GameObject(L"Basic/Collider.hlsl")
{
	SetColor(0, 1, 0);
}

void Collider::Render()
{
	if (!isDraw) return;

	GameObject::Render(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
}

bool Collider::IsCollision(Collider* collider)
{
    if (!IsActive() || !collider->IsActive()) return false;	

	switch (collider->type)
	{
	case Collider::BOX:
		return IsBoxCollision((BoxCollider*)collider);
	case Collider::SPHERE:
		return IsSphereCollision((SphereCollider*)collider);
	case Collider::CAPSULE:
		return IsCapsuleCollision((CapsuleCollider*)collider);	
	}

    return false;
}

bool Collider::Push(Collider* collider, RaycastHit* hit)
{
	if (!IsActive() || !collider->IsActive()) return false;

	switch (collider->type)
	{
	case Collider::BOX:
		return PushBox((BoxCollider*)collider, hit);
	case Collider::SPHERE:
		return PushSphere((SphereCollider*)collider, hit);
	case Collider::CAPSULE:
		return PushCapsule((CapsuleCollider*)collider, hit);
	}

	return false;
}
