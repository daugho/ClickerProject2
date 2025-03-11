#pragma once

struct OreData
{
	int key = 0;
	string name;
	int hp;
	int price;
	wstring textureFile;
	wstring iconFile;
};

struct CraftData
{
	int key = 0;
	int row, col;
	int recipe[3][3];
	int result;
};

class Block : public Cube
{
public:
	Block(int key);
	~Block();

	void PostRender();

	void Damage();

	int GetHp() { return curHp; }

	OreData GetData() { return data; }
private:	
	ProgressBar* hpBar;

	OreData data;
	int curHp;

	Vector3 hpBarOffset = { 0, 0.6f, 0.0f };
};