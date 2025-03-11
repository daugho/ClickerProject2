#include "Framework.h"

FloorManager::FloorManager(Vector3 position) : floorPos(position)
{
	//CreatPrview();
	objectManager = new ObjectManager();
	floortileBuffer = new FloatValueBuffer();
	material->SetShader(L"Custom/MapEditor.hlsl");
	SetLocalScale(1, 1, 1);
	SetLocalPosition(position);
	collider = new BoxCollider(Vector3(floormapSize.x, 1, floormapSize.y));
	collider->SetParent(this);
	collider->SetLocalPosition(Vector3(0, 0, 0));
	InitializerFloor(floormapSize.x, floormapSize.y);
}

FloorManager::~FloorManager()
{
	delete floortileBuffer;
	delete collider;  // ? `collider` 삭제 추가
	delete objectManager;
	// ? 바닥 오브젝트 삭제
	for (GameObject* object : floorsobjects)
		delete object;
	floorsobjects.clear();  // ? 리스트 정리

	// ? 천장 오브젝트 삭제
	for (GameObject* ceiling : ceilingCubes)
		delete ceiling;
	ceilingCubes.clear();  // ? 리스트 정리

	// ? floorpreviews의 모든 오브젝트 삭제
	for (pair<FloorEditType, GameObject*> preview : floorpreviews)
		delete preview.second;
	floorpreviews.clear();  // ? 리스트 정리

	// ? 바닥 및 천장 상태 정보 초기화
	floorOccupied.clear();
	floortypes.clear();
}

void FloorManager::Render()
{
	floortileBuffer->SetPS(10);

	collider->Render();

	Cube::Render();

	for (GameObject* obj : floorsobjects)
		obj->Render();

	Environment::Get()->SetAdditive();
	for (pair<FloorEditType, GameObject*> preview : floorpreviews)
		preview.second->Render();
	Environment::Get()->SetAlphaBlend(false);

	if (showCeiling)
	{
		for (GameObject* cube : ceilingCubes)
			cube->Render();
	}
}

void FloorManager::Edit()
{
	ImGui::Begin("Floor Manager Setting");

	ImGui::Text("FloorMapEditor");

	ImGui::DragInt2("SelectTile", (int*)&floorselectTile);

	if (ImGui::DragInt2("MapSize", (int*)&floormapSize), 1, 1, 100)
	{
		//size = Vector3(floormapSize.x, 1.0f, floormapSize.y);

		UpdateMesh();
		InitializerFloor(floormapSize.x, floormapSize.y);
	}

	if (ImGui::DragInt2("Tiling", (int*)&floormapTiling), 1, 1, 100)
	{
		tiling = floormapTiling;
		UpdateMesh();
	}

	material->Edit();

	const char* list[] = {"Wall", "Hermit", "Ore", "Item" };
	if (ImGui::Combo("Type", (int*)&floorEditType, list, 4))
	{
	}
	if (floorEditType == FOre)
	{
		const char* orelist[] = { "Ore0", "Ore1", "Ore2", "Ore3", "Ore4", "Ore5" };
		ImGui::Combo("OreType", &oreID, orelist, 6);
	}
		ImGui::Text("CeilingEditor");

		ImGui::Checkbox("Show Ceiling", &showCeiling);

		if (ImGui::DragInt2("ceilingSize", (int*)&cilingSize), 1, 1, 100)
		{
			if (showCeiling) UpdateCeilingMesh();
		}

		if (ImGui::DragInt2("ceilingTiling", (int*)&cilingTiling), 1, 1, 100)
		{
			tiling = cilingTiling;
			if (showCeiling) UpdateCeilingMesh();
		}
		if (ImGui::Button("WorldSave"))
		{
			Save();
		}
		ImGui::SameLine();
		if (ImGui::Button("WorldLoad"))
		{
			Load();
		}
		ImGui::End();
}

void FloorManager::Update()
{
	UpdateWorld();
	SetSelectTile();
	SetEdit();
	collider->Update();
	for (pair<FloorEditType, GameObject*> preview : floorpreviews)
		preview.second->Update();
}

void FloorManager::UpdateMesh()
{
	if (floormapSize.x == 0 || floormapSize.y == 0)
		return;

	size = Vector3(floormapSize.x, 1.0f, floormapSize.y);

	SetVertices();
	MakeNormal();
	MakeTangent();
	mesh->UpdateVertices();
	collider->UpdateMesh(size);

	floortileBuffer->Get()[2] = 1.0f / floormapSize.x * floormapTiling.x;
	floortileBuffer->Get()[3] = 1.0f / floormapSize.y * floormapTiling.y;

	floortypes.resize(floormapSize.x * floormapSize.y);
}

void FloorManager::UpdateCeilingMesh()
{
	if (!showCeiling) return;


	ceilingCubes.clear();

	Cube* ceiling = new Cube(Vector3(cilingSize.x, 1, cilingSize.y), Vector2(cilingTiling.x, cilingTiling.y));
	ceiling->SetLocalPosition(Vector3(floorPos.x, floorPos.y + 7.0f, floorPos.z));
	ceiling->GetMaterial()->SetDiffuseMap(L"Resources/Textures/Block/T_Ore1_Color.png");
	ceiling->SetVertices();
	ceiling->MakeNormal();
	ceiling->MakeTangent();
	ceiling->Update();
	mesh->UpdateVertices();

	ceilingCubes.push_back(ceiling);
}

void FloorManager::SetEdit()
{
	if (KEY->Down(VK_LBUTTON) && !ImGui::GetIO().WantCaptureMouse)
	{
		switch (floorEditType)
		{
		case FloorManager::FWall:
			CreatWall();
			break;
		case FloorManager::FHermit:
			objectManager->CreateHermit(floorselectTile,floormapSize);
			break;
		case FloorManager::FOre:
			objectManager->CreateOre(floorselectTile, floormapSize,oreID);
			break;
		case FloorManager::FItem:
			break;
		default:
			break;
		}
	}
}

void FloorManager::SetSelectTile()
{
	Ray ray = CAM->ScreenPointToRay(mousePos);
	RaycastHit hit;

	if (collider->IsRayCollision(ray, &hit))
	{
		Vector2 size = floormapSize;
		Vector2 pos = { hit.point.x, hit.point.z };
		pos += size * 0.5f;
		floorselectTile = { (int)pos.x, (int)pos.y };

		floortileBuffer->Get()[0] = (float)floorselectTile.x / floormapSize.x * floormapTiling.x;
		floortileBuffer->Get()[1] = (floormapSize.y - (float)floorselectTile.y - 1) / floormapSize.y * floormapTiling.y;

		if (floorpreviews.count(floorEditType) > 0)
		{
			float y = 0.0f;

			switch (floorEditType)
			{
			case FloorManager::FWall:
				y = 1.0f;
				break;
			case FloorManager::FHermit:
				break;
			case FloorManager::FOre:
				break;
			case FloorManager::FItem:
				break;
			default:
				break;
			}

			Vector3 startPos = Vector3(floormapSize.x * -0.5f, 0.0f, floormapSize.y * -0.5f);
			Vector3 pos = startPos + Vector3(floorselectTile.x + 0.5f, y, floorselectTile.y + 0.5f);
			floorpreviews[floorEditType]->SetLocalPosition(pos);
		}
	}
}

void FloorManager::CreatPrview()
{
	Cube* wall = new Cube({ 1, 3, 1 });
	wall->GetMaterial()->SetShader(L"Custom/Preview.hlsl");
	wall->GetMaterial()->SetDiffuseMap(L"Resources/Textures/Landscape/Dirt3.png");
	wall->GetMaterial()->GetData()->emissive = { 0, 1, 1, 0.5f };
	wall->SetActive(true);

	floorpreviews[FWall] = wall;

	Hermit* hermit = new Hermit("machine");
	hermit->GetMaterial()->SetShader(L"Custom/Preview.hlsl");
	hermit->GetMaterial()->GetData()->emissive = { 0, 1, 1, 0.5f };
	hermit->SetActive(true);
	floorpreviews[FHermit] = hermit;

	Ore* ore = new Ore(0, "MinerOre");
	ore->GetMaterial()->SetShader(L"Custom/Preview.hlsl");
	ore->GetMaterial()->GetData()->emissive = { 0, 1, 1, 0.5f };
	ore->SetActive(true);
	floorpreviews[FOre] = ore;
}

void FloorManager::CreatWall()
{
	Vector3 startPos = Vector3(floormapSize.x * -0.5f, 0.0f, floormapSize.y * -0.5f);
	Vector3 pos = startPos + Vector3(floorselectTile.x + 0.5f, 2.0f, floorselectTile.y + 0.5f);

	int index = floorselectTile.y * floormapSize.x + floorselectTile.x;

	
	int x = floorselectTile.x- (int)floorPos.x;
	int y = floorselectTile.y-(int)floorPos.y;

	if (floorOccupied[y][x]) return;

	Cube* wall = new Cube({ 1, 3, 1 });
	wall->SetLocalPosition(pos);
	wall->GetMaterial()->SetDiffuseMap(L"Resources/Textures/Landscape/Dirt3.png");
	wall->Update();

	floorsobjects.push_back(wall);

	//floorobjectData[index] = wall;

	floortypes[y][x] = FWall;
	floorOccupied[y][x] = true;
}

void FloorManager::InitializerFloor(int width, int height)
{
	if (width <= 0 || height <= 0) {return;}
	floorOccupied.resize(height, vector<bool>(width, false));//설치가 되어있는지 확인.
	floortypes.resize(height, vector<FloorEditType>(width, FloorEditType::None));//무엇이 있는지 확인.
}

void FloorManager::Save()
{
	BinaryWriter* writer = new BinaryWriter("Resources/Data/FloorData.map");

	// 맵 크기 저장
	writer->Data<POINT>(floormapSize);

	// 저장할 오브젝트 개수 기록
	int objectCount = (int)floorobjectData.size();
	writer->Data<int>(objectCount);

	// 모든 오브젝트 정보 저장
	for (auto& obj : floorobjectData)
	{
		int index = obj.first; // 위치 정보
		GameObject* gameObject = obj.second;

		int x = index % floormapSize.x; // X 좌표 변환
		int y = index / floormapSize.x; // Y 좌표 변환

		FloorEditType type = floortypes[y][x]; // 2D 접근 방식으로 수정

		writer->Data<int>(type); // 타입 저장
		writer->Data<int>(index); // 위치 정보 저장

		// Ore일 경우, oreID도 저장
		if (type == FOre)
		{
			Ore* ore = dynamic_cast<Ore*>(gameObject);
			if (ore)
			{
				writer->Data<int>(ore->GetData().id); // oreID 저장
			}
		}
	}

	delete writer;
}

void FloorManager::Load()
{
	BinaryReader* reader = new BinaryReader("Resources/Data/FloorData.map");

	// 맵 크기 불러오기
	floormapSize = reader->Data<POINT>();

	// 기존 데이터 초기화
	floortypes.clear();
	floortypes.resize(floormapSize.y, vector<FloorEditType>(floormapSize.x, None));

	// 오브젝트 개수 불러오기
	int objectCount = reader->Data<int>();

	for (int i = 0; i < objectCount; i++)
	{
		int type = reader->Data<int>();  // 타입 불러오기
		int index = reader->Data<int>(); // 위치 정보 불러오기

		int x = index % floormapSize.x; // X 좌표 변환
		int y = index / floormapSize.x; // Y 좌표 변환

		floortypes[y][x] = static_cast<FloorEditType>(type); // 올바르게 2D 벡터에 저장

		POINT selectTile = { x, y };

		switch ((FloorEditType)type)
		{
		case FOre:
		{
			int oreID = reader->Data<int>();
			objectManager->CreateOre(selectTile, floormapSize, oreID);
			break;
		}
		case FHermit:
			objectManager->CreateHermit(selectTile, floormapSize);
			break;
		case FWall:
			CreatWall();
			break;
		default:
			break;
		}
	}

	delete reader;
}
