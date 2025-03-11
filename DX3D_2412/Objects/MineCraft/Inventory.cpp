#include "Framework.h"
#include "Inventory.h"

Inventory::Inventory() : Quad(L"Resources/Textures/UI/Inventory.png")
{
	localPosition = CENTER;
	UpdateWorld();

	CreateSlots();

	Observer::Get()->AddParamEvent("OnSelectSlot", bind(&Inventory::OnSelectSlot, this, placeholders::_1));
}

Inventory::~Inventory()
{	
	for (InventorySlot* slot : slots)
		delete slot;

	for (InventorySlot* slot : craftSlots)
		delete slot;
}

void Inventory::Update()
{
	if (!IsActive()) return;

	CheckCraftItem();

	selectSlot->SetLocalPosition(mousePos);

	for (InventorySlot* slot : slots)
		slot->Update();

	for (InventorySlot* slot : craftSlots)
		slot->Update();
}

void Inventory::Render()
{
	if (!IsActive()) return;

	Quad::Render();

	for (InventorySlot* slot : slots)
		slot->Render();

	for (InventorySlot* slot : craftSlots)
		slot->Render();
}

void Inventory::Edit()
{
	//for (InventorySlot* slot : slots)
	//	slot->Edit();

	for (InventorySlot* slot : craftSlots)
		slot->Edit();
}

int Inventory::AddItem(int key)
{
	if (blockData.count(key) > 0)
	{
		blockData[key]->IncreaseCount();		
		return blockData[key]->GetIndex();
	}

	OreData data = BlockManager::Get()->GetOreData(key);

	InventorySlot* icon = slots[insertIndex];
	icon->SetData(data, insertIndex);

	blockData[key] = icon;

	do
	{
		insertIndex++;
	} while (slots[insertIndex]->GetCount() != 0);
}

void Inventory::DeleteItem(int key)
{
}

void Inventory::CreateSlots()
{
	InventorySlot* slot = new InventorySlot();
	slot->SetTag("InventorySlot0");
	slot->SetParent(this);	
	slot->Load();
	slots.push_back(slot);

	float interval = 4.0f;

	for(int i = 1 ; i < 9 ; i++)
	{
		InventorySlot* slot = new InventorySlot();
		Vector3 pos = slots[0]->GetLocalPosition() + Vector3::Right() * ((slot->Size().x + interval) * i);
		slot->SetLocalPosition(pos);
		slot->SetParent(this);
		slot->UpdateWorld();

		slots.push_back(slot);
	}

	slot = new InventorySlot();
	slot->SetTag("InventorySlot10");
	slot->SetParent(this);
	slot->Load();
	slots.push_back(slot);

	for (int i = 1; i < 27; i++)
	{
		InventorySlot* slot = new InventorySlot();
		float right = (slot->Size().x + interval) * (i % 9);
		float down = (slot->Size().y + interval) * (i / 9);
		Vector3 pos = slots[9]->GetLocalPosition() + Vector3::Right() * right + Vector3::Down() * down;
		slot->SetLocalPosition(pos);	
		slot->SetParent(this);
		slot->UpdateWorld();

		slots.push_back(slot);
	}

	selectSlot = new InventorySlot();
	selectSlot->SetActive(false);
	selectSlot->SetDrag(true);	
	slots.push_back(selectSlot);

	//for (InventorySlot* slot : slots)
	//{
	//	slot->SetParamEvent(bind(&Inventory::OnSelectSlot, this, placeholders::_1));
	//	slot->SetObject(slot);
	//}

	slot = new InventorySlot();
	slot->SetTag("CraftSlot0");
	slot->SetParent(this);
	slot->Load();	

	craftSlots.push_back(slot);	
	
	for (int i = 1; i < 4; i++)
	{
		InventorySlot* slot = new InventorySlot();
		float right = (slot->Size().x + interval) * (i % 2);
		float down = (slot->Size().y + interval) * (i / 2);
		Vector3 pos = craftSlots[0]->GetLocalPosition() + Vector3::Right() * right + Vector3::Down() * down;
		slot->SetLocalPosition(pos);		
		slot->SetParent(this);
		slot->UpdateWorld();
	
		craftSlots.push_back(slot);
	}

	resultSlot = new InventorySlot();
	resultSlot->SetTag("ResultSlot");
	resultSlot->SetParent(this);
	resultSlot->Load();

	craftSlots.push_back(resultSlot);
}

void Inventory::OnSelectSlot(void* inventorySlot)
{
	InventorySlot* temp = (InventorySlot*)inventorySlot;

	if (!selectSlot->IsActive() && temp->GetCount() > 0)
	{
		selectSlot->SetActive(true);
		selectSlot->Swap(temp);
		UIManager::Get()->RefreshQuickSlotIcons();
		return;
	}

	selectSlot->Swap(temp);		

	if (selectSlot->GetCount() == 0)
		selectSlot->SetActive(false);

	UIManager::Get()->RefreshQuickSlotIcons();
}

void Inventory::CheckCraftItem()
{
	FOR(4)
	{
		int row = i % 2;
		int col = i / 2;

		craftItems[row][col] = { craftSlots[i]->GetData().key, craftSlots[i]->GetCount() };
	}

	for (int i = BlockManager::Get()->GetCraftDataCount(); i > 0; i--)
	{
		CraftData craftData = BlockManager::Get()->GetCraftDatas(i);

		


		resultSlot->SetActive(false);
	}

	for (int y = 0; y < 2; y++)
	{
		for (int x = 0; x < 2; x++)
		{

		}
	}
}

bool Inventory::IsCraftable(CraftData craftData)
{
	for (int y = 0; y < 3 - craftData.row; y++)
	{
		for (int x = 0; x < 3 - craftData.col; x++)
		{
			int startX = x;
			int startY = y;

			for (int r = 0; r < craftData.row; r++)
			{
				for (int c = 0; r < craftData.col; c++)
				{
					int crartKey = craftItems[startX + c][startY + r].first;
					int recipeKey = craftData.recipe[c][r];

					if (crartKey != recipeKey)
					{						
						return false;
					}
				}
			}
		}
	}

	return false;
}
