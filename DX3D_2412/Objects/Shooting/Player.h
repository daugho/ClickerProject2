#pragma once

class OreManager;
struct DropData;
class OreInventory;
class Player : public SphereCollider
{
private:
	const float GRAVITY = 9.8f;
	const float JUMP_POWER = 10.0f;

public:
	Player();
	~Player();

	void Update();
	void Render();
	void PostRender();

	void Mining();
	bool TakeItem(DropData item, int count);
	void ToggleInventory();
private:
	void Control();

	void Jump();
	void Move();

	void SetCursor();


private:
	float moveSpeed = 5.0f;
	float rotSpeed = 1.0f;
	int playerDamage = 1;

	Vector3 velocity;

	POINT clientCenterPos;

	LightBuffer::Light* light;

	OreInventory* oreInventory;

	float miningCooldown = 1.0f;  // ? Ã¤±¼ Äð´Ù¿î (ÃÊ´ç 1È¸)
	float miningTimer = 0.0f;

	bool isInventoryOpen = false;
};