#include "Framework.h"
#include "CollisionScene.h"

CollisionScene::CollisionScene()
{
	//colliders.push_back(new SphereCollider());
	//colliders.back()->SetTag("Sphere1");

	//colliders.push_back(new BoxCollider());
	//colliders.back()->SetTag("Box1");

	//colliders.push_back(new SphereCollider());
	//colliders.back()->SetTag("Sphere2");

	//colliders.push_back(new BoxCollider());
	//colliders.back()->SetTag("Box2");

	colliders.push_back(new CapsuleCollider());
	colliders.back()->SetTag("Capsule1");

	colliders.push_back(new CapsuleCollider());
	colliders.back()->SetTag("Capsule2");

	//colliders.push_back(new SphereCollider());
	//colliders.back()->SetTag("Sphere2");
}

CollisionScene::~CollisionScene()
{
	for (Collider* collider : colliders)
		delete collider;
}

void CollisionScene::Update()
{
	if (colliders[0]->IsCollision(colliders[1]))
	{
		colliders[0]->SetColor(1, 0, 0);
		colliders[1]->SetColor(1, 0, 0);
	}
	else
	{
		colliders[0]->SetColor(0, 1, 0);
		colliders[1]->SetColor(0, 1, 0);
	}

	//Ray ray = CAM->ScreenPointToRay(mousePos);
	//RaycastHit hit;
	//
	//if (colliders[0]->IsRayCollision(ray, &hit))
	//{
	//	colliders[0]->SetColor(1, 0, 0);
	//
	//	//Vector3 pos = hit.point + hit.normal * 3;
	//	//colliders[1]->SetLocalPosition(pos);
	//	colliders[1]->SetLocalPosition(hit.point);
	//}
	//else
	//{
	//	colliders[0]->SetColor(0, 1, 0);
	//}

	for (Collider* collider : colliders)
		collider->UpdateWorld();
}

void CollisionScene::PreRender()
{
}

void CollisionScene::Render()
{
	for (Collider* collider : colliders)
		collider->Render();
}

void CollisionScene::PostRender()
{
}

void CollisionScene::GUIRender()
{
	for (Collider* collider : colliders)
		collider->Edit();
}
