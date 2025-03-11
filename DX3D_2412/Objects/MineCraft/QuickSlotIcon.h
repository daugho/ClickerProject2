#pragma once

class QuickSlotIcon : public Quad
{
public:
	QuickSlotIcon();
	~QuickSlotIcon();

	void Render();

	void SetData(OreData data, int count);

	void IncreaseCount() { count++; }
	void DecreaseCount() { count--; }
	int GetCount() { return count; }	
	int GetIndex() { return index; }

private:
	OreData oreData;
	int count = 0;
	int index = 0;
};