#include "Framework.h"

CubeMapEditor::CubeMapEditor()
{	
	//CreatePreview();

	tileBuffer = new FloatValueBuffer();

	Load();
	material->SetShader(L"Custom/MapEditor.hlsl");	
	objectManager = new ObjectManager();
	//OreManager::Get()->CreatOre(selectTile, mapSize, 0);
}

CubeMapEditor::~CubeMapEditor()
{	
	for (FloorManager* floor : floorManagers)
		delete floor;
	delete tileBuffer;
	delete objectManager;
	for (GameObject* object : objects)
		delete object;

	//for (pair<EditType, GameObject*> preview : previews)
	//	delete preview.second;
	OreManager::Delete();
}

void CubeMapEditor::Update()
{
	SetSelectTile();
	SetEdit();	
	if (player != nullptr)
	{
		vector<BoxCollider*> hitCoins = objectManager->GetCollisionObjects(5.0f);

		for (BoxCollider* collider : objectManager->GetAllColliders())
		{
			if (find(hitCoins.begin(), hitCoins.end(), collider) != hitCoins.end())
			{
				collider->SetColor(Float4(1.0f, 0.0f, 0.0f, 1.0f));
			}
			else
			{
				collider->SetColor(Float4(1, 1, 1, 1));
			}
		}
	}
	if (KEY->Down(VK_NUMPAD7))
	{
		CreatePlayer();
		CAM->SetTarget(player);
		CAM->TargetOptionLoad("FPSMode");
	}
	if (player != nullptr)
		player->Update();
	for (pair<EditType, GameObject*> preview : previews)
		preview.second->Update();
	for (FloorManager* floor : floorManagers)
		floor->Update();
	objectManager->Update();
	OreManager::Get()->Update();
}

void CubeMapEditor::Render()
{	
	for (FloorManager* floor : floorManagers)
		floor->Render();
	objectManager->Render();
	OreManager::Get()->Render();
	tileBuffer->SetPS(10);
	//Cube::Render();
	for (GameObject* object : objects)
		object->Render();
	Environment::Get()->SetAdditive();
	for (pair<EditType, GameObject*> preview : previews)
		preview.second->Render();
	Environment::Get()->SetAlphaBlend(true);
	if (player != nullptr)
		player->Render();
	Environment::Get()->SetAlphaBlend(false);
}

void CubeMapEditor::Edit()
{
	ImGui::Text("MapEditor");

	if (ImGui::Button("Add Floor"))
	{
		static int width = 0;
		static int height = 0;
		const int maxWidth = 3;

		FloorManager* floor = new FloorManager(Vector3(width * 25.0f, 0, height * 25.0f));

		floorManagers.push_back(floor);

		width++;

		if (width >= maxWidth)
		{
			width = 0;
			height++;
		}
	}

	if (ImGui::TreeNode("Floor Managers"))
	{
		for (int i = 0; i < floorManagers.size(); i++)
		{
			string label = "Floor " + to_string(i + 1);
			if (ImGui::TreeNode(label.c_str()))
			{
				floorManagers[i]->Edit();

				if (ImGui::Button("Remove Floor"))
				{
					delete floorManagers[i];
					floorManagers.erase(floorManagers.begin() + i);
					ImGui::TreePop();
					break;
				}

				ImGui::TreePop();
			}
		}

		ImGui::TreePop();
	}

	const char* list[] = {"None", "Wall", "Hermit", "StartPos", "Ore", "Item"};
	if (ImGui::Combo("Type", (int*)&editType, list, 6))
	{
		for(pair<EditType, GameObject*> preview : previews)
		{
			if (editType == preview.first)
			{
				preview.second->SetActive(true);
			}
			else
			{
				preview.second->SetActive(false);
			}
		}
	}

	if (ImGui::Button("Save"))
	{
		Save();
	}
	ImGui::SameLine();
	if (ImGui::Button("Load"))
	{
		Load();
	}
	material->Edit();
	objectManager->Edit();
}

void CubeMapEditor::SetSelectTile()
{
	Ray ray = CAM->ScreenPointToRay(mousePos);
	RaycastHit hit;

	if (collider->IsRayCollision(ray, &hit))
	{
		Vector2 size = mapSize;
		Vector2 pos = { hit.point.x, hit.point.z };
		pos += size * 0.5f;
		selectTile = { (int)pos.x, (int)pos.y };

		tileBuffer->Get()[0] = (float)selectTile.x / mapSize.x * mapTiling.x;
		tileBuffer->Get()[1] = (mapSize.y - (float)selectTile.y - 1) / mapSize.y * mapTiling.y;

		if (previews.count(editType) > 0)
		{
			float y = 0.0f;

			switch (editType)
			{
			case CubeMapEditor::Wall:
				y = 2.0f;
				break;
			case CubeMapEditor::Coin:
				y = 1.0f;
				break;
			case CubeMapEditor::StartPos:
				break;
			case CubeMapEditor::Ore:
				break;
			case CubeMapEditor::Item:

				break;
			default:
				break;
			}

			Vector3 startPos = Vector3(mapSize.x * -0.5f, 0.0f, mapSize.y * -0.5f);
			Vector3 pos = startPos + Vector3(selectTile.x + 0.5f, y, selectTile.y + 0.5f);
			previews[editType]->SetLocalPosition(pos);
		}
	}
}

void CubeMapEditor::DeleteObject()
{
	int index = selectTile.y * mapSize.x + selectTile.x;

	if (objectData.count(index) == 0)
		return;

	list<GameObject*>::iterator deleteObject = find(objects.begin(), objects.end(), objectData[index]);

	objects.erase(deleteObject);
	objectData.erase(index);
}

void CubeMapEditor::SetEdit()
{
	if (KEY->Down(VK_LBUTTON) && !ImGui::GetIO().WantCaptureMouse)
	{
		switch (editType)
		{
		case CubeMapEditor::Wall:
			//objectManager->CreateWall(selectTile, mapSize);
			break;
		case CubeMapEditor::Coin:
			objectManager->CreateHermit(selectTile, mapSize);
			break;
		case CubeMapEditor::StartPos:
			break;
		case CubeMapEditor::Ore:
			objectManager->CreateOre(selectTile, mapSize, oreID);
			break;
		case CubeMapEditor::Item:
			break;
		default:
			break;
		}
	}

	if (KEY->Down(VK_RBUTTON))
	{
		DeleteObject();
	}
}

void CubeMapEditor::CreatePlayer()
{
	if (player != nullptr)
	{
		return;
	}

	player = new Player();
	player->SetLocalPosition(Vector3(5, 5, 5));
	player->UpdateWorld();

}

void CubeMapEditor::UpdateMesh()
{
	if (mapSize.x == 0 || mapSize.y == 0)
		return;

	SetVertices();
	MakeNormal();
	MakeTangent();
	mesh->UpdateVertices();
	collider->UpdateMesh(size);

	tileBuffer->Get()[2] = 1.0f / mapSize.x * mapTiling.x;
	tileBuffer->Get()[3] = 1.0f / mapSize.y * mapTiling.y;

	types.resize(mapSize.x * mapSize.y);
}


void CubeMapEditor::Save()
{
	BinaryWriter* writer = new BinaryWriter("Resources/TextData/Pacman.map");

	writer->Data<POINT>(mapSize);
	writer->Data<POINT>(mapTiling);

	writer->Byte(types.data(), sizeof(EditType) * types.size());

	delete writer;
}

void CubeMapEditor::Load()
{
	BinaryReader* reader = new BinaryReader("Resources/TextData/Pacman.map");

	mapSize = reader->Data<POINT>();
	mapTiling = reader->Data<POINT>();

	material->Load("Resources/Materials/PacmanMap.mat");

	UpdateMesh();

	delete reader;
}
