#pragma once
class OreInventory;

class ClickerUIManager : public Singleton<ClickerUIManager>
{
    friend class Singleton;
public:
    ClickerUIManager();
    ~ClickerUIManager();

    void Update();
    void Render();
    void Edit();
    //void ShowMiningEffect();

private:
    void CreateUI();  // UI ��� ����

private:
    bool isInventoryOpen = false;

    Quad* cursor;
    Quad* miningEffect;
    Quad* resourcePanel;

    OreInventory* inventory;

    BlendState* blendState[2];
};