#pragma once

class Enemy : public SphereCollider
{
public:
	Enemy();
	~Enemy();

	void Update();
	void Render();
	void PostRender();

private:
	Transform* target;

	Cube* cube;
};
