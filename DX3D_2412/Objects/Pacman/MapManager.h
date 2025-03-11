#pragma once

class MapManager : public Singleton<MapManager>
{
private:
	friend class Singleton;

	MapManager();
	~MapManager();

public:
	void Update();
	void Render();

	void Load(string file);

private:
	Cube* floor;
	vector<Cube*> walls;
};