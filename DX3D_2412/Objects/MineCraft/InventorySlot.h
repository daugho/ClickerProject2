#pragma once

class Inventory;

class InventorySlot : public Button
{
public:
	InventorySlot();
	~InventorySlot();

	void Update();
	void Render();

	void SetData(OreData data, int index);
	void UpdateData();
	void Swap(InventorySlot* slot);

	void IncreaseCount() { count++; }
	void DecreaseCount() { count--; }
	int GetCount() { return count; }
	int GetIndex() { return index; }
	void SetDrag(bool isDrag) { this->isDrag = isDrag; }


	OreData GetData() { return oreData; }

private:
	void OnClickSlot();	

private:
	OreData oreData;
	int count = 0;
	int index = 0;

	bool isDrag = false;	
};