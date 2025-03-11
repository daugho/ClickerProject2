#include "Framework.h"

OreSlot::OreSlot() : Button(Vector2(60,60))
{
	image->SetActive(false);
	//SetActive(false);
}

OreSlot::~OreSlot()
{
}

void OreSlot::Update()
{
	Button::Update();
}

void OreSlot::SetItem(const DropData& item, int count)
{
    this->item = item;
    this->count = count;
    occupied = true;
    SetActive(true);

	UpdateData();
}

void OreSlot::Render()
{
	Collider::Render();
	image->Render();

	if (count == 0)
		return;

	Font::Get()->SetStyle("Icon");
	Font::Get()->SetColor("Yellow");
	Font::Get()->RenderText(to_string(count), GetGlobalPosition());
}

void OreSlot::ShowTooltip()
{

}

void OreSlot::SetData(DropData data, int index)
{
	image->SetActive(true);
	item = data;
	image->GetMaterial()->SetDiffuseMap(item.texturePath);
	this->index = index;
}

void OreSlot::UpdateData()
{
	if (count == 0)
	{
		image->SetActive(false);
		return;
	}

	image->SetActive(true);
	image->GetMaterial()->SetDiffuseMap(item.texturePath);
}
