#include "Framework.h"

OreManager::OreManager()
{
    LoadOreTable("Resources/Tables/OreData.csv");
    for (const auto& ore : oreTable)
    {
        LoadDropTable(ore.first);
    }

}
OreManager::~OreManager()
{

}

void OreManager::Update()
{
}

void OreManager::Render()
{
    for (Ore* ore : ores)
        ore->Render();
}

void OreManager::CreatOre(POINT selectTile, POINT mapSize, int oreID)
{

    Vector3 startPos = Vector3(mapSize.x * -0.5f, 0.0f, mapSize.y * -0.5f);
    Vector3 pos = startPos + Vector3(selectTile.x + 0.5f, 1.0f, selectTile.y + 0.5f);

    int index = selectTile.y * mapSize.x + selectTile.x;
    string defaultModel = "MinerOre";
    Ore* ore = new Ore(oreID, defaultModel);
    ore->SetLocalPosition(pos);
    ore->Update();

    ores.push_back(ore);
    oredata[index] = ore;
}

void OreManager::LoadOreTable(string path)
{
    FILE* file;
    fopen_s(&file, path.c_str(), "r");

    bool isFirstLine = true;

    while (!feof(file))
    {
        char temp[1024];
        fgets(temp, 1024, file);

        if (isFirstLine)
        {
            isFirstLine = false;
            continue;
        }

        string row = temp;
        Utility::Replace(row, "\n", "");

        vector<string> datas = Utility::SplitString(row, ",");

        oreData data;
        data.id = stoi(datas[0]);
        data.name = datas[1];
        data.texturePath = Utility::ToWString(datas[2]);
        data.size = Vector3(stof(datas[3]), stof(datas[4]), stof(datas[5]));
        data.health = stoi(datas[6]);


        oreTable[data.id] = data;
    }
    fclose(file);
}

void OreManager::LoadDropTable(int oreID)
{
    string path = "Resources/Tables/Ore" + to_string(oreID) + "DropTable.csv";

    FILE* file;
    fopen_s(&file, path.c_str(), "r");

    if (!file)
    {
        printf("Failed to open DropTable file: %s\n", path.c_str());
        return;
    }

    bool isFirstLine = true;

    while (!feof(file))
    {
        char temp[1024];
        fgets(temp, 1024, file);

        if (isFirstLine)
        {
            isFirstLine = false;
            continue;
        }

        string row = temp;
        Utility::Replace(row, "\n", "");

        vector<string> datas = Utility::SplitString(row, ",");

        if (datas.size() < 7) // 필드 개수 확인
        {
            printf("Invalid DropTable row: %s\n", row.c_str());
            continue;
        }

        DropData data;
        data.id = stoi(datas[0]);  // oreID
        data.name = datas[1];
        data.texturePath = Utility::ToWString(datas[2]);
        data.dropRate = stof(datas[3]);
        data.minCount = stoi(datas[4]);
        data.maxCount = stoi(datas[5]);
        data.price = stoi(datas[6]);

        dropTable[oreID].push_back(data); // ? oreID별 DropTable 저장
    }
    fclose(file);
}

Ore* OreManager::GetClosestOre(const Ray& ray, RaycastHit* hit)
{
    Ore* closestOre = nullptr;
    float minDistance = FLT_MAX;

    for (Ore* ore : ores)
    {
        RaycastHit tempHit;
        if (ore->IsRayCollision(ray, &tempHit))
        {
            float distance = Vector3::Distance(ray.origin, tempHit.point);
            if (distance < minDistance)
            {
                minDistance = distance;
                closestOre = ore;
                *hit = tempHit;
            }
        }
    }

    for (Ore* ore : ores)
    {
        ore->SetColliderColor(Float4(1, 1, 1, 1));  // 원래 색상
    }

    // ? 충돌한 Ore만 빨간색으로 변경
    if (closestOre)
    {
        closestOre->SetColliderColor(Float4(1, 0, 0, 1));  // 빨간색
    }

    return closestOre;
}

