#include "Framework.h"

Bullet::Bullet(Transform* transform)
	: SphereCollider(0.2f), transform(transform)
{
	SetTag(transform->GetTag() + "_Collider");
	transform->SetParent(this);
	transform->SetTag("Arrow_0");
	transform->Load();
}

Bullet::~Bullet()
{	
}

void Bullet::Update()
{
	if (!IsActive()) return;

	lifeTime += DELTA;

	if (lifeTime > LIFE_TIME)
		SetActive(false);

	Translate(velocity * speed * DELTA);

	UpdateWorld();	
}

void Bullet::Render()
{
	Collider::Render();	
}

void Bullet::Edit()
{
	//transform->Edit();
	Transform::Edit();
}

void Bullet::Fire(Vector3 pos, Vector3 dir)
{
	localPosition = pos;
	velocity = dir;
	lifeTime = 0.0f;

	localRotation.y = atan2(dir.x, dir.z);

	SetActive(true);
}
