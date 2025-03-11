#include "Framework.h"

UIManager::UIManager()
{
	Font::Get()->AddStyle("Icon", L"배달의민족 을지로체 TTF", 40);
	Font::Get()->AddColor("Yellow", 0, 1, 1);

	CreateInGameUI();

	inventory = new Inventory();
	inventory->SetActive(false);
}

UIManager::~UIManager()
{
	delete cursor;
	delete hotBar;

	for (Quad* icon : blockIcons)
		delete icon;
}

void UIManager::Update()
{
	if (KEY->Down('I'))
	{
		isPopup = !isPopup;
		ShowCursor(isPopup);
		inventory->SetActive(isPopup);
	}

	SelectBuildBlock();

	//hotBar->UpdateWorld();
	//for (Quad* icon : blockIcons)
	//	icon->UpdateWorld();

	SelectBlock();

	inventory->Update();
}

void UIManager::Render()
{
	cursor->Render();
	hotBar->Render();
	hotBarSelect->Render();	

	for (QuickSlotIcon* icon : blockIcons)
		icon->Render();

	inventory->Render();
}

void UIManager::Edit()
{
	//hotBar->Edit();
	//
	//for (Quad* icon : blockIcons)
	//	icon->Edit();

	inventory->Edit();
}

void UIManager::Mining()
{
	if (selectBlock == nullptr) return;	

	selectBlock->Damage();

	if (selectBlock->GetHp() == 0)
	{
		selectBlock->SetActive(false);

		int key = selectBlock->GetData().key;
		int index = inventory->AddItem(key);

		RefreshQuickSlotIcons();
	}	
}

void UIManager::Build()
{
	if (!blockIcons[selectBuildBlockIndex]->IsActive()) return;

	//Block* block = (Block*)mineBlocks[selectBuildBlockIndex].back();
	//
	//if (BlockManager::Get()->BuildBlock(block, MINING_DISTANCE))
	//{
	//	mineBlocks[selectBuildBlockIndex].pop_back();
	//	blockData[block->GetData().key]->DecreaseCount();
	//
	//	if (mineBlocks[selectBuildBlockIndex].empty())
	//	{
	//		blockIcons[selectBuildBlockIndex]->SetActive(false);
	//		mineBlocks.erase(selectBuildBlockIndex);
	//		blockData.erase(block->GetData().key);
	//
	//		if (insertIndex > selectBuildBlockIndex)
	//			insertIndex = selectBuildBlockIndex;
	//	}
	//}
}

void UIManager::RefreshQuickSlotIcons()
{
	FOR(blockIcons.size())
	{
		InventorySlot* slot = inventory->GetSlot(i);
		blockIcons[i]->SetData(slot->GetData(), slot->GetCount());
	}
}

void UIManager::SelectBlock()
{
	selectBlock = (Block*)BlockManager::Get()->GetCollisionBlock(MINING_DISTANCE);

	if (selectBlock)
		cursor->GetMaterial()->GetData()->diffuse = { 1, 0, 0, 1 };
	else
		cursor->GetMaterial()->GetData()->diffuse = { 1, 1, 1, 1 };
}

void UIManager::SelectBuildBlock()
{
	FOR(9)
	{
		int key = '1';

		if (KEY->Down(key + i))
		{
			selectBuildBlockIndex = i;			
			hotBarSelect->SetLocalPosition(blockIcons[i]->GetGlobalPosition());
			hotBarSelect->UpdateWorld();
		}
	}
}

void UIManager::OpenInventory()
{
}

void UIManager::CreateInGameUI()
{
	cursor = new Quad(L"Resources/Textures/UI/cursor.png");
	cursor->SetLocalPosition(CENTER);
	cursor->UpdateWorld();

	hotBar = new Quad(L"Resources/Textures/UI/HotBar.png");
	hotBar->SetTag("HotBar");
	hotBar->Load();

	FOR(9)
	{
		QuickSlotIcon* icon = new QuickSlotIcon();
		icon->SetTag("Slot" + to_string(i));
		icon->SetParent(hotBar);		

		if (i == 0)
		{
			icon->Load();
		}
		else
		{
			icon->SetLocalScale(blockIcons[0]->GetLocalScale());
			Vector3 pos = blockIcons[0]->GetLocalPosition();
			pos.x += 20.0f * i;
			icon->SetLocalPosition(pos);
			icon->UpdateWorld();
		}

		icon->SetActive(false);

		blockIcons.push_back(icon);
	}

	hotBarSelect = new Quad(L"Resources/Textures/UI/HotBarSelect.png");
	hotBarSelect->SetLocalScale(3, 3, 3);
	hotBarSelect->SetLocalPosition(blockIcons[0]->GetGlobalPosition());
	hotBarSelect->UpdateWorld();
}
