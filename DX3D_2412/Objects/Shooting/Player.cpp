#include "Framework.h"

Player::Player()
{
	tag = "Player";

	localPosition.y = radius;

	clientCenterPos = { SCREEN_WIDTH >> 1, SCREEN_HEIGHT >> 1 };
	ClientToScreen(hWnd, &clientCenterPos);
	SetCursorPos(clientCenterPos.x, clientCenterPos.y);
	ShowCursor(false);

	//CAM->SetLocalPosition(0, 0, 0);
	//CAM->SetLocalRotation(0, 0, 0);
	//CAM->SetParent(this);

	light = Environment::Get()->GetLight(0);
	light->type = 2;
	light->color = { 1, 0, 0, 1 };

	oreInventory = new OreInventory();
}

Player::~Player()
{

}

void Player::Update()
{
	if (UIManager::Get()->IsPopup())
		return;

	light->position = localPosition;
	light->direction = CAM->GetForward();

	SetCursor();
	Control();

	Jump();
	Move();

	UpdateWorld();
	Mining();
	ToggleInventory();

}

void Player::Render()
{
	Collider::Render();

}

void Player::PostRender()
{
}

void Player::Mining()
{
	miningTimer += DELTA;  // ? �� ������ Ÿ�̸� ����

	if (miningTimer < miningCooldown)  // ? ��ٿ��� ���������� ���� �� ��
		return;

	if (KEY->Press(VK_LBUTTON))  // ? ���콺 ��Ŭ���� ������ ä�� ����
	{
		Ray ray = CAM->ScreenPointToRay(CENTER);
		RaycastHit hit;

		Ore* closestOre = OreManager::Get()->GetClosestOre(ray, &hit);  // ? ���� ����� ���� ã��

		if (closestOre)
		{
			closestOre->TakeDamage(playerDamage);  // ? ü�� ���� (ä��)
			miningTimer = 0.0f;  // ? Ÿ�̸� �ʱ�ȭ (��ٿ� ����)
		}
	}
}

bool Player::TakeItem(DropData item, int count)
{
	if (!oreInventory)
		return false;

	if (oreInventory->IsFull())
		return false;
	return oreInventory->AddItem(item,count);
}

void Player::ToggleInventory()
{
	if (KEY->Down('I'))
	{
		isInventoryOpen = !isInventoryOpen;
		oreInventory->SetActive(isInventoryOpen);
	}
}

void Player::Control()
{
	Vector3 dir;

	if (KEY->Press('W'))
		dir += GetForward();
	if (KEY->Press('S'))
		dir += GetBack();
	if (KEY->Press('A'))
		dir += GetLeft();
	if (KEY->Press('D'))
		dir += GetRight();

	dir.Normalize();

	velocity.x = dir.x;
	velocity.z = dir.z;

	Vector3 delta = mousePos - CENTER;

	Rotate(Vector3::Up(), delta.x * rotSpeed * DELTA);
	CAM->Rotate(Vector3::Left(), delta.y * rotSpeed * DELTA);

	if (KEY->Down(VK_SPACE))
	{
		velocity.y = JUMP_POWER;
	}
}



void Player::Jump()
{
	velocity.y -= GRAVITY * DELTA;

	float bottomHeight = BlockManager::Get()->GetHeight(localPosition);

	if (velocity.y < 0 && localPosition.y - radius <= bottomHeight)
	{
		velocity.y = 0.0f;
	}
}

void Player::Move()
{
	if (UIManager::Get()->IsPopup())
		return;

	Translate(velocity * moveSpeed * DELTA);
}

void Player::SetCursor()
{
	//if(UIManager::Get()->)

	SetCursorPos(clientCenterPos.x, clientCenterPos.y);
}

