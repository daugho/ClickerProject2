#pragma once
class ObjectManater;
class FloorManager : public Cube
{
private:
	enum FloorEditType
	{
		FWall, FHermit, FOre , FItem , None
	};
public:
	FloorManager(Vector3 position);
	~FloorManager();

	void Render();
	void Edit();
	void Update();
	void UpdateMesh();
	void UpdateCeilingMesh();
	void SetEdit();
	void SetSelectTile();
	void CreatPrview();
	void CreatWall();


	void InitializerFloor(int width, int height);

	void Save();
	void Load();
private:
	ObjectManager* objectManager;
	int floorCount = 2;
	POINT floormapTiling = { 1,1 };
	POINT floormapSize = {12,12};
	POINT floorselectTile;

	POINT cilingTiling = { 1,1 };
	POINT cilingSize = { 12,12 };

	FloorEditType floorEditType;

	FloatValueBuffer* floortileBuffer;

	list<GameObject*> floorsobjects;
	map<int, GameObject*> floorobjectData;

	vector<vector<FloorEditType>> floortypes;

	map<FloorEditType, GameObject*> floorpreviews;

	vector<vector<bool>> floorOccupied;

	Vector3 floorPos;

	vector<GameObject*> ceilingCubes;
	bool showCeiling = false;

	int oreID = 0;
};