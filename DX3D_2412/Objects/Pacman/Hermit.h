#pragma once

class CapsuleCollider;

class Hermit : public Cube
{
public:
	Hermit(string modelName);
	~Hermit();

	void Render() ; 
	void Update() ;
	void ByeOre();
	void SellItem();
	void Edit();
	bool IsRayCollision(const Ray& ray, RaycastHit* hit);

private:

	Model* model;
	BoxCollider* collider;
	bool showCollider = true;
};