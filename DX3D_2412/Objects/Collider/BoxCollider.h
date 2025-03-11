#pragma once

class BoxCollider : public Collider
{
public:
	struct ObbDesc
	{
		Vector3 center;
		Vector3 axis[3];
		Vector3 halfSize;
	};

public:
	BoxCollider(Vector3 size = { 1, 1, 1 });

	virtual bool IsRayCollision(IN const Ray& ray, OUT RaycastHit* hit) override;
	virtual bool IsBoxCollision(BoxCollider* collider) override;
	virtual bool IsSphereCollision(SphereCollider* collider) override;
	virtual bool IsCapsuleCollision(CapsuleCollider* collider) override;
	
	virtual bool PushBox(BoxCollider* collider, RaycastHit* hit) override;
	virtual bool PushSphere(SphereCollider* collider, RaycastHit* hit) override;
	virtual bool PushCapsule(CapsuleCollider* collider, RaycastHit* hit) override;

	void GetObb(ObbDesc& obbDesc);

	void UpdateMesh(const Vector3& size);

private:
	virtual void MakeMesh() override;

	bool IsSeperateAxis(const Vector3& D, const Vector3& axis,
		const ObbDesc& box1, const ObbDesc& box2);

private:
	Vector3 size;
};