#include "Framework.h"

QuickSlotIcon::QuickSlotIcon() : Quad(L"Resources/Textures/Block/Block0.png")
{
}

QuickSlotIcon::~QuickSlotIcon()
{
}

void QuickSlotIcon::Render()
{
	if (!IsActive()) return;

	Quad::Render();
	

	Font::Get()->SetStyle("Icon");
	Font::Get()->SetColor("Yellow");
	Font::Get()->RenderText(to_string(count), GetGlobalPosition());
}

void QuickSlotIcon::SetData(OreData data, int count)
{
	if (data.key == 0)
	{
		SetActive(false);
		return;
	}

	SetActive(true);

	oreData = data;
	material->SetDiffuseMap(oreData.iconFile);
	this->count = count;	
}
