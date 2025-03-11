#pragma once

class SphereCollider : public Collider
{
public:
	SphereCollider(float radius = 1.0f, UINT stackCount = 8, UINT sliceCount = 16);

	virtual bool IsRayCollision(IN const Ray& ray, OUT RaycastHit* hit) override;
	virtual bool IsBoxCollision(BoxCollider* collider) override;
	virtual bool IsSphereCollision(SphereCollider* collider) override;
	virtual bool IsCapsuleCollision(CapsuleCollider* collider) override;
	
	virtual bool PushBox(BoxCollider* collider, RaycastHit* hit) override;
	virtual bool PushSphere(SphereCollider* collider, RaycastHit* hit) override;
	virtual bool PushCapsule(CapsuleCollider* collider, RaycastHit* hit) override;

	float Radius()
	{
		return radius * max(GetGlobalScale().x,
			max(GetGlobalScale().y, GetGlobalScale().z));
	}

private:
	virtual void MakeMesh() override;

protected:
	float radius;

private:	
	UINT stackCount, sliceCount;
};