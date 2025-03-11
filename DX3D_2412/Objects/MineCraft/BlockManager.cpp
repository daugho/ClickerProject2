#include "Framework.h"

BlockManager::BlockManager()
{
    LoadOreData("Resources/Tables/OreTable.csv");
    LoadCraftData("Resources/Tables/CraftTable.csv");
}

BlockManager::~BlockManager()
{
    for (Cube* block : blocks)
        delete block;

    blocks.clear();
}

void BlockManager::Update()
{
}

void BlockManager::Render()
{
    for (Cube* block : blocks)
        block->Render();
}

void BlockManager::PostRender()
{
    //for (Block* block : blocks)
    //    block->PostRender();
}

void BlockManager::CreateBlocks(UINT width, UINT height)
{
    for (UINT z = 0; z < height; z++)
    {
        for (UINT x = 0; x < width; x++)
        {
            Vector3 pos = { (float)x, 0, (float)z };

            int key = GameMath::Random(1, 6);

            Block* block = new Block(key);            
            block->SetLocalPosition(pos);
            block->Update();

            blocks.push_back(block);
        }
    }
}

Cube* BlockManager::GetCollisionBlock(float distance)
{
    Ray ray = CAM->ScreenPointToRay(CENTER);

    float minDistance = FLT_MAX;
    RaycastHit hit;
    Cube* collisionBlock = nullptr;

    for (Cube* block : blocks)
    {
        float dist = Vector3::Distance(block->GetGlobalPosition(), CAM->GetGlobalPosition());

        if (dist > distance)
            continue;

        if (block->GetCollider()->IsRayCollision(ray, &hit))
        {
            if (minDistance > hit.distance)
            {
                collisionBlock = block;
                minDistance = hit.distance;
            }
        }
    }

    return collisionBlock;
}

bool BlockManager::BuildBlock(Cube* block, float distance)
{
    Ray ray = CAM->ScreenPointToRay(CENTER);
        
    RaycastHit hit;
    RaycastHit minHit;
    minHit.distance = FLT_MAX;

    Cube* collisionBlock = nullptr;

    for (Cube* block : blocks)
    {
        float dist = Vector3::Distance(block->GetGlobalPosition(), CAM->GetGlobalPosition());

        if (dist > distance)
            continue;

        if (block->GetCollider()->IsRayCollision(ray, &hit))
        {
            if (hit.distance < minHit.distance)
            {
                minHit = hit;
                collisionBlock = block;
            }
        }
    }

    if (collisionBlock == nullptr) return false;

    block->SetActive(true);
    block->SetLocalPosition(collisionBlock->GetGlobalPosition() + hit.normal);
    block->Update();

    return true;
}

float BlockManager::GetHeight(const Vector3& pos) const
{
    Ray ray(pos, Vector3::Down());
    RaycastHit hit;

    float maxHeight = 0.0f;

    for (Cube* block : blocks)
    {
        if (block->GetCollider()->IsRayCollision(ray, &hit))
        {
            if (hit.point.y > maxHeight)
                maxHeight = hit.point.y;
        }
    }

    return maxHeight;
}

void BlockManager::LoadOreData(string path)
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

        OreData data;
        data.key = stoi(datas[0]);
        data.name = datas[1];
        data.hp = stoi(datas[2]);
        data.price = stoi(datas[3]);
        data.textureFile = Utility::ToWString(datas[4]);
        data.iconFile = Utility::ToWString(datas[5]);

        oreDatas[data.key] = data;
    }    
}

void BlockManager::LoadCraftData(string path)
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

        CraftData data;
        data.key = stoi(datas[0]);
        data.row = stoi(datas[1]);
        data.col = stoi(datas[2]);
        data.recipe[0][0] = stoi(datas[3]);
        data.recipe[1][0] = stoi(datas[4]);
        data.recipe[2][0] = stoi(datas[5]);
        data.recipe[0][1] = stoi(datas[6]);
        data.recipe[1][1] = stoi(datas[7]);
        data.recipe[2][1] = stoi(datas[8]);
        data.recipe[0][2] = stoi(datas[9]);
        data.recipe[1][2] = stoi(datas[10]);
        data.recipe[2][2] = stoi(datas[11]);
        data.result = stoi(datas[12]);        

        craftDatas[data.key] = data;
    }
}