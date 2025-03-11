#pragma once

class OreSlot : public Button {
public:
    OreSlot();
    ~OreSlot();

    void Update();
    void SetItem(const DropData& item, int count);
    void Render();
    void ShowTooltip();

    void SetData(DropData data,int index);
    bool IsOccupied() const { return occupied; }
    const DropData& GetItem() const { return item; }
    int GetCount() const { return count; }

    void UpdateData();

private:
    DropData item;
    int count = 0;
    bool occupied = false;
    int index = 0;
};