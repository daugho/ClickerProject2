#include "Framework.h"

Planet::Planet(float rotationSpeed, float revolutionSpeed, float parentToDistance, Transform* parent)
	: revolutionSpeed(revolutionSpeed), rotationSpeed(rotationSpeed),
	parentToDistance(parentToDistance)
{
	SetParent(parent);
	Translate(Vector3::Right() * parentToDistance);
}

void Planet::Update()
{
	Rotate(Vector3(1, 1, 0), rotationSpeed * DELTA);

	//if (parent)
	//{
	//	revolutionAngle += revolutionSpeed * DELTA;
	//
	//	float x = cos(revolutionAngle) * parentToDistance;
	//	float z = sin(revolutionAngle) * parentToDistance;
	//
	//	localPosition = parent->GetLocalPosition() + Vector3(x, 0, z);
	//}

	UpdateWorld();
}
