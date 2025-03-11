#pragma once
class OreSlot;
class OreInventory : public Quad
{
public:
    OreInventory();
    ~OreInventory();

    void Update();
    void Render();
    void Edit();
    void CreatSlot();

    bool AddItem(const DropData& item, int count);
    bool IsFull() const;
 

    bool IsActive() const { return isActive; }

private:

    int currentCapacity = 0;
    const int MAX_CAPACITY = 10;
    const int START_SLOTS = 5;

    const int GRID_ROWS = 3;
    const int GRID_COLS = 5;
    const int TOTAL_SLOTS = GRID_ROWS * GRID_COLS;

    vector<OreSlot*> slots;

    bool isActive = false;
private:
    struct InventorySlot 
    {
        DropData item;// 아이템 데이터
        int count;
        bool occupied = false;
    };
};