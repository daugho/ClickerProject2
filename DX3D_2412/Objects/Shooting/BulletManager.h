#pragma once

class BulletManager : public Singleton<BulletManager>
{
private:
	const UINT SIZE = 50;

private:
	friend class Singleton;

	BulletManager();
	~BulletManager();

public:
	void Update();
	void Render();
	void Edit();

	void Fire(Vector3 pos, Vector3 direction);

private:
	ModelInstancing* modelInstancing;

	vector<Bullet*> bullets;
};