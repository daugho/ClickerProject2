#pragma once

class UIManager : public Singleton<UIManager>
{
private:
	friend class Singleton;

	const float MINING_DISTANCE = 3.0f;

private:
	UIManager();
	~UIManager();

public:
	void Update();
	void Render();	
	void Edit();

	void Mining();
	void Build();

	bool IsPopup() { return isPopup; }

	void RefreshQuickSlotIcons();

private:
	void SelectBlock();
	void SelectBuildBlock();

	void OpenInventory();
	void CreateInGameUI();

private:
	Quad* cursor;
	Quad* hotBar;
	Quad* hotBarSelect;
	
	vector<QuickSlotIcon*> blockIcons;	
	Block* selectBlock;

	bool isPopup = false;	
	int selectBuildBlockIndex = 0;

	Inventory* inventory;
};