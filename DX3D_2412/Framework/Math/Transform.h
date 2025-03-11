#pragma once

class Transform
{
public:
	Transform();

	virtual void Edit();
	void Save();
	void Load();

	void UpdateWorld();

	void Translate(const Vector3& translation) { localPosition += translation; }
	void Translate(const float& x, const float& y, const float& z)
	{
		localPosition += Vector3(x, y, z);
	}
	
	void Rotate(Vector3 axis, const float& angle) { localRotation += axis * angle; }
	void Rotate(const float& x, const float& y, const float& z)
	{
		localRotation += Vector3(x, y, z);
	}

	Vector3 GetRight() const { return right.GetNormalized(); }
	Vector3 GetLeft() const { return -right.GetNormalized(); }
	Vector3 GetUp() const { return up.GetNormalized(); }
	Vector3 GetDown() const { return -up.GetNormalized(); }
	Vector3 GetForward() const { return forward.GetNormalized(); }
	Vector3 GetBack() const { return -forward.GetNormalized(); }

	Vector3 GetLocalPosition() const { return localPosition; }
	Vector3 GetLocalRotation() const { return localRotation; }
	Vector3 GetLocalScale() const { return localScale; }

	Vector3 GetGlobalPosition() const { return globalPosition; }
	Vector3 GetGlobalRotation() const { return globalRotation; }
	Vector3 GetGlobalScale() const { return globalScale; }

	Matrix GetWorld() { return world; }

	void SetLocalPosition(const Vector3& position) { localPosition = position; }
	void SetLocalRotation(const Vector3& rotation) { localRotation = rotation; }
	void SetLocalScale(const Vector3& scale) { localScale = scale; }

	void SetGlobalPosition(const Vector3& position);

	void SetLocalPosition(const float& x, const float& y, const float& z) { localPosition = Vector3(x, y, z); }
	void SetLocalRotation(const float& x, const float& y, const float& z) { localRotation = Vector3(x, y, z); }
	void SetLocalScale(const float& x, const float& y, const float& z) { localScale = Vector3(x, y, z); }

	void SetParent(Transform* parent) { this->parent = parent; }
	void SetPivot(const Vector3& pivot) { this->pivot = pivot; }
	void SetPivot(const float& x, const float& y, const float& z) { this->pivot = Vector3(x, y, z); }

	string GetTag() { return tag; }
	void SetTag(string tag) { this->tag = tag; }
	bool IsActive();

	void SetActive(bool isActive) { this->isActive = isActive; }		

protected:
	string name;
	string tag;
	bool isActive = true;

	Matrix world;

	Vector3 localPosition;
	Vector3 localRotation;
	Vector3 localScale = { 1, 1, 1 };

private:
	Transform* parent = nullptr;

	Vector3 pivot;

	Vector3 right = { 1, 0, 0 };
	Vector3 up = { 0, 1, 0 };
	Vector3 forward = { 0, 0, 1 };
	Vector3 globalPosition, globalRotation;
	Vector3 globalScale = { 1, 1, 1 };

	Float4x4 matWorld;

};
