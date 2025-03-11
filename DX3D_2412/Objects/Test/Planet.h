#pragma once

class Planet : public Cube
{
public:
	Planet(float rotationSpeed, float revolutionSpeed = 0.0f, 
		float parentToDistance = 0.0f, Transform* parent = nullptr);

	void Update();

private:
	float revolutionSpeed;
	float rotationSpeed;
	float parentToDistance;

	float revolutionAngle = 0.0f;
};