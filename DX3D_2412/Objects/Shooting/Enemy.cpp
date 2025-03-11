#include "Framework.h"

Enemy::Enemy()
{
	cube = new Cube(Vector3(1.5f, 1.5f, 1.5f));
	cube->SetParent(this);
}

Enemy::~Enemy()
{
	delete cube;
}

void Enemy::Update()
{
	UpdateWorld();
	cube->UpdateWorld();
}

void Enemy::Render()
{
	Collider::Render();
	cube->Render();
}

void Enemy::PostRender()
{
	Vector2 pos = CAM->WorldToScreenPoint(localPosition);		
	Font::Get()->RenderText("Enemy", pos);
}
