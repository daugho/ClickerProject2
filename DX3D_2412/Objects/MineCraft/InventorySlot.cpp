#include "Framework.h"
#include "InventorySlot.h"

InventorySlot::InventorySlot() : Button(Vector2(32, 32))
{	
	image->SetActive(false);

	SetEvent(bind(&InventorySlot::OnClickSlot, this));
}

InventorySlot::~InventorySlot()
{
}

void InventorySlot::Update()
{
	Button::Update();	
}

void InventorySlot::Render()
{
	Collider::Render();
	image->Render();

	if (count == 0)
		return;

	Font::Get()->SetStyle("Icon");
	Font::Get()->SetColor("Yellow");
	Font::Get()->RenderText(to_string(count), GetGlobalPosition());
}

void InventorySlot::SetData(OreData data, int index)
{
	image->SetActive(true);
	oreData = data;
	image->GetMaterial()->SetDiffuseMap(oreData.iconFile);
	count = 1;
	this->index = index;
}

void InventorySlot::Swap(InventorySlot* slot)
{
	OreData tempData = slot->oreData;
	slot->oreData = oreData;
	oreData = tempData;	

	int tempCount = slot->count;
	slot->count = count;
	count = tempCount;

	UpdateData();
	slot->UpdateData();


}

void InventorySlot::OnClickSlot()
{
	if (isDrag)
		return;	

	Observer::Get()->ExcuteParamEvent("OnSelectSlot", this);
}

void InventorySlot::UpdateData()
{
	if (count == 0)
	{
		image->SetActive(false);
		return;
	}

	image->SetActive(true);	
	image->GetMaterial()->SetDiffuseMap(oreData.iconFile);		
}
