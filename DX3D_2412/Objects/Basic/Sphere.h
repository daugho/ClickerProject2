#pragma once

class Sphere : public GameObject<VertexUVNormalTangent>
{
private:
	typedef VertexUVNormalTangent VertexType;

public:
	Sphere(float radius = 1.0f, UINT sliceCount = 32, UINT stackCount = 16);	

	float Radius() { return radius; }

private:
	void MakeMesh();	

private:
	float radius;
	UINT sliceCount, stackCount;
};