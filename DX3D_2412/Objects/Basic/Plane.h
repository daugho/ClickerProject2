#pragma once

class Plane : public GameObject<VertexUVNormalTangent>
{
private:
	typedef VertexUVNormalTangent VertexType;

public:
	Plane(Vector2 size, UINT row, UINT col);
	
private:
	void MakeMesh();

private:
	Vector2 size;
	UINT row, col;
};