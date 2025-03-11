#pragma once

class BlockManager : public Singleton<BlockManager>
{
private:
	friend class Singleton;

	BlockManager();
	~BlockManager();

public:
	void Update();
	void Render();
	void PostRender();

	void CreateBlocks(UINT width, UINT height);

	Cube* GetCollisionBlock(float distance);
	bool BuildBlock(Cube* block, float distance);

	float GetHeight(const Vector3& pos) const;

	OreData GetOreData(int key) { return oreDatas[key]; }
	UINT GetOreDataCount() { return oreDatas.size(); }

	CraftData GetCraftDatas(int key) { return craftDatas[key]; }
	UINT GetCraftDataCount() { return craftDatas.size(); }
private:
	void LoadOreData(string path);
	void LoadCraftData(string path);

private:
	vector<Block*> blocks;

	unordered_map<int, OreData> oreDatas;
	unordered_map<int, CraftData> craftDatas;	
};