#include "Framework.h"

Block::Block(int key)
{
	data = BlockManager::Get()->GetOreData(key);
	curHp = data.hp;

	material->SetDiffuseMap(data.textureFile);

	hpBar = new ProgressBar(L"Resources/Textures/UI/enemy_hp_bar.png", L"Resources/Textures/UI/enemy_hp_bar_BG.png");
}

Block::~Block()
{
}

void Block::PostRender()
{
	Vector3 pos = CAM->WorldToScreenPoint(localPosition + hpBarOffset);
	Font::Get()->RenderText(data.name, pos);	
	
	hpBar->SetLocalPosition(pos);
	hpBar->UpdateWorld();
	hpBar->Render();
}

void Block::Damage()
{
	curHp--;

	hpBar->SetAmount((float)curHp / data.hp);
}

