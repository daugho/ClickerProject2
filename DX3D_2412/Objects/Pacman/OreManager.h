#pragma once

struct oreData
{
    int id;
    string name;
    wstring texturePath;
    Vector3 size;
    int health;
};

struct DropData
{
    int id;
    string name;
    wstring texturePath;
    float dropRate;
    int minCount;
    int maxCount;
    int price;
};

class OreManager : public Singleton<OreManager>
{
    friend class Singleton;
    friend class Ore;
public:
    OreManager();
    ~OreManager();
public:
    void Update();
    void Render();
    void CreatOre(POINT selectTile, POINT mapSize, int oreID);
    void LoadOreTable(string path);
    void LoadDropTable(int oreID);
    oreData GetOreData(int oreID) { return oreTable[oreID];}
    vector<DropData>& GetDropData(int oreID) { return dropTable[oreID];}

    Ore* GetClosestOre(const Ray& ray, RaycastHit* hit);

private:
    SphereCollider* collider;
    //Ore* ore;
    unordered_map<int, oreData> oreTable;
    unordered_map<int, vector<DropData>> dropTable;

    list<Ore*> ores;
    map<int, Ore*> oredata;

};