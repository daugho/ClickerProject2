#include "Framework.h"

ObjectManager::ObjectManager()
{
}

ObjectManager::~ObjectManager()
{
	for (GameObject* wall : objects)
		delete wall;
	for (Hermit* hermit : hermits)
		delete hermit;
	for (BoxCollider* col : colliders)
		delete col;
	for (CapsuleCollider* cap : capcolliders)
		delete cap;
}

void ObjectManager::CreateWall(POINT selectTile, POINT mapSize)
{
	Vector3 startPos = Vector3(mapSize.x * -0.5f, 0.0f, mapSize.y * -0.5f);
	Vector3 pos = startPos + Vector3(selectTile.x + 0.5f, 2.0f, selectTile.y + 0.5f);

	int index = selectTile.y * mapSize.x + selectTile.x;

	if (objectData.count(index) > 0)
		return;

	Cube* wall = new Cube({ 1, 3, 1 });
	wall->SetLocalPosition(pos);
	wall->GetMaterial()->SetDiffuseMap(L"Resources/Textures/Landscape/Dirt3.png");
	wall->Update();

	objects.push_back(wall);
	objectData[index] = wall;
}

void ObjectManager::CreateHermit(POINT selectTile, POINT mapSize)
{

	string oreNum = "Hermit" + to_string(hermitCount);
	Vector3 startPos = Vector3(mapSize.x * -0.5f, 0.0f, mapSize.y * -0.5f);
	Vector3 pos = startPos + Vector3(selectTile.x + 0.5f, 0.5f, selectTile.y + 0.5f);

	int index = selectTile.y * mapSize.x + selectTile.x;

	if (objectData.count(index) > 0)
		return;

	Hermit* hermit = new Hermit("machine");
	hermit->SetLocalPosition(pos);
	hermit->SetTag(oreNum);
	hermit->Update();

	hermits.push_back(hermit);
	hermitdata[index] = hermit;
	hermitCount++;
}

void ObjectManager::CreateOre(POINT selectTile, POINT mapSize, int oreID)
{
	OreManager::Get()->CreatOre(selectTile, mapSize, oreID);
}

void ObjectManager::DeleteObject(int index)
{
}

void ObjectManager::Render()
{
	for (GameObject* wall : objects)
		wall->Render();
	for (Hermit* hermit : hermits)
		hermit->Render();
	for (BoxCollider* col : colliders)
		col->Render();
	for (CapsuleCollider* cap : capcolliders)
		cap->Render();
}

void ObjectManager::Edit()
{

	for (GameObject* obj : objects)
	{
		obj->Edit();
	}
	for (Hermit* hermit : hermits)
		hermit->Edit();
	for (BoxCollider* col : colliders)
		col->Edit();
	for (CapsuleCollider* cap : capcolliders)
		cap->Edit();
}

vector<BoxCollider*> ObjectManager::GetCollisionObjects(float distance)
{
	Ray ray = CAM->ScreenPointToRay(CENTER);

	vector<BoxCollider*> hitCoins;
	RaycastHit hit;

	for (BoxCollider* collider : colliders)
	{
		float dist = Vector3::Distance(collider->GetGlobalPosition(), CAM->GetGlobalPosition());

		if (dist > distance)
			continue;

		if (collider->IsRayCollision(ray, &hit))
		{
			hitCoins.push_back(collider);
		}
	}

	return hitCoins;
}
