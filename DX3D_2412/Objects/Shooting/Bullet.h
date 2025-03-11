#pragma once

class Bullet : public SphereCollider
{
private:
	const float LIFE_TIME = 3.0f;

public:
	Bullet(Transform* transform);
	~Bullet();

	void Update();
	void Render();
	void Edit();

	void Fire(Vector3 pos, Vector3 dir);

private:
	Transform* transform;

	float speed = 5.0f;
	Vector3 velocity;
	float lifeTime = 0.0f;
};