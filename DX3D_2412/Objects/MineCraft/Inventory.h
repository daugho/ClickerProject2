#pragma once

class Inventory : public Quad
{
public:
	Inventory();
	~Inventory();

	void Update();
	void Render();
	void Edit();

	int AddItem(int key);
	void DeleteItem(int key);

	InventorySlot* GetSlot(int index) { return slots[index]; }
private:
	void CreateSlots();	
	void OnSelectSlot(void* inventorySlot);

	void CheckCraftItem();
	bool IsCraftable(CraftData craftData);

private:
	vector<InventorySlot*> slots;
	unordered_map<int, InventorySlot*> blockData;

	vector<InventorySlot*> craftSlots;

	int insertIndex = 0;

	InventorySlot* selectSlot;
	InventorySlot* resultSlot;

	pair<int, int> craftItems[2][2];
};