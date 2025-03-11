#include "Framework.h"

OreInventory::OreInventory() : Quad(L"Resources/Textures/UI/Bag.png")
{
    localPosition = CENTER;
    UpdateWorld();
    CreatSlot();
}

OreInventory::~OreInventory()
{
    for (OreSlot* slot : slots) {
        delete slot;
    }
}

void OreInventory::Update()
{
    UpdateWorld();
    for (OreSlot* slot : slots) {
        slot->Update();
    }
}

void OreInventory::Render()
{
    //if (!isActive) return;

    Quad::Render();

    for (OreSlot* slot : slots) {
        slot->Render();
    }
}

void OreInventory::Edit()
{
    //for (OreSlot* slot : slots) {
    //    slot->Edit();
    //}
}

void OreInventory::CreatSlot()
{
    OreSlot* slot = new OreSlot();
    slot->SetTag("Ore_InventorySlot");
    slot->SetParent(this);
    slot->Load();
    slots.push_back(slot);
    
    float interval = 7.0f;
    float  Hinterval = 5.0f;
    for (int i = 1; i < 5; i++)
    {
        OreSlot* slot = new OreSlot();
        float xOffset = (slot->Size().x + interval) * i;

        Vector3 pos = slots[0]->GetLocalPosition() + Vector3(xOffset, 0, 0);
        slot->SetLocalPosition(pos);
        slot->SetParent(this);
        slot->UpdateWorld();

        slots.push_back(slot);
    }


    for (int j = 1; j < 3; j++)
    {
        for (int i = 0; i < 5; i++)
        {
            OreSlot* slot = new OreSlot();
            float xOffset = (slot->Size().x + interval) * i;
            float yOffset = (slot->Size().y + Hinterval) * -j;

            Vector3 pos = slots[0]->GetLocalPosition() + Vector3(xOffset, yOffset, 0);
            slot->SetLocalPosition(pos);
            slot->SetParent(this);
            slot->UpdateWorld();

            slots.push_back(slot);
        }
    }
}

bool OreInventory::AddItem(const DropData& item, int count)
{
    for (auto& slot : slots) {
        if (slot->IsOccupied() && slot->GetItem().id == item.id) {
            int availableSpace = MAX_CAPACITY - slot->GetCount();

            if (availableSpace >= count) {
                slot->SetItem(item, slot->GetCount() + count);
                return true;
            }
            else {
                slot->SetItem(item, MAX_CAPACITY);
                return false;
            }
        }
    }


    for (auto& slot : slots) {
        if (!slot->IsOccupied()) {
            slot->SetItem(item, count);
            return true;
        }
    }

    return false;
}

bool OreInventory::IsFull() const
{
    return currentCapacity >= MAX_CAPACITY;
}
