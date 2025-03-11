#pragma once

class Hermit;
class ObjectManager;
class FloorManager;
class OreManager;
class CubeMapEditor : public Cube
{
private:
	enum EditType
	{
		None, Wall, Coin, StartPos, Ore, Item
	};

public:
	CubeMapEditor();
	~CubeMapEditor();

	void Update();
	void Render();
	void Edit();

private:
	void SetSelectTile();
	void CreateStartPos();
	void CreatePreview();
	void DeleteObject();


	void SetEdit();
	void CreatePlayer();
	void UpdateMesh();

	void Save();
	void Load();

private:	

	ObjectManager* objectManager;
	FloorManager* floorManager;
	OreManager* oreManager;
	POINT mapSize;
	POINT mapTiling = { 1, 1 };
	POINT selectTile;
	int oreID = 0;
	int floorNum = 0;

	EditType editType = Wall;

	FloatValueBuffer* tileBuffer;

	list<GameObject*> objects;
	map<int, GameObject*> objectData;

	vector<EditType> types;

	map<EditType, GameObject*> previews;

	Player* player = nullptr;

	vector<FloorManager*> floorManagers;
};