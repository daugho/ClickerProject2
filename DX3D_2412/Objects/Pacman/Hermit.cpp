#include "Framework.h"

Hermit::Hermit(string modelName) : Cube(Vector3(1,2,1))
{
	model = new Model(modelName);
	model->SetParent(this);

	collider = new BoxCollider(Vector3(1,2,1));
	collider->SetParent(this);

}

Hermit::~Hermit()
{
	delete collider;
}

void Hermit::Render()
{
	model->Render();
	collider->Render();
}

void Hermit::Update()
{
	UpdateWorld();
	if (model)
		model->UpdateWorld();
	if (collider)
	{
		collider->SetLocalPosition(Vector3(0, 1, 0));
		collider->UpdateWorld();
	}
}

void Hermit::ByeOre()
{
}

void Hermit::SellItem()
{
}

void Hermit::Edit()
{
	model->Edit();
}

bool Hermit::IsRayCollision(const Ray& ray, RaycastHit* hit)
{
	return collider->IsRayCollision(ray, hit);
}
