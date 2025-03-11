#include "Framework.h"

BulletManager::BulletManager()
{
	modelInstancing = new ModelInstancing("Arrow", SIZE);

	bullets.reserve(SIZE);

	FOR(SIZE)
	{
		Bullet* bullet = new Bullet(modelInstancing->Add());
		bullet->SetActive(false);

		bullets.push_back(bullet);
	}
}

BulletManager::~BulletManager()
{
	delete modelInstancing;
}

void BulletManager::Update()
{
	for (Bullet* bullet : bullets)
		bullet->Update();

	modelInstancing->Update();
}

void BulletManager::Render()
{
	for (Bullet* bullet : bullets)
		bullet->Render();

	modelInstancing->Render();
}

void BulletManager::Edit()
{
	modelInstancing->Edit();

	for (Bullet* bullet : bullets)
		bullet->Edit();
}

void BulletManager::Fire(Vector3 pos, Vector3 direction)
{
	for (Bullet* bullet : bullets)
	{
		if (!bullet->IsActive())
		{
			bullet->Fire(pos, direction);
			return;
		}
	}
}
