#include "Framework.h"

Plane::Plane(Vector2 size, UINT row, UINT col)
	: size(size), row(row), col(col)
{
	tag = "Plane";

	MakeMesh();
	MakeNormal();
	MakeTangent();
	mesh->CreateMesh();
}

void Plane::MakeMesh()
{
    vector<VertexType>& vertices = mesh->GetVertices();
	vertices.reserve(row * col);

	float dx = size.x / (col - 1);
	float dz = size.y / (row - 1);

	for (UINT z = 0; z < row; z++)
	{
		for (UINT x = 0; x < col; x++)
		{
			VertexType vertex;
			vertex.pos = { x * dx, 0.0f, z * dz };
			vertex.uv.x = x / (float)(col - 1);
			vertex.uv.y = z / (float)(row - 1);

			vertices.push_back(vertex);
		}
	}

    vector<UINT>& indices = mesh->GetIndices();

	indices.reserve((row - 1) * (col - 1) * 6);

	for (UINT z = 0; z < row - 1; z++)
	{
		for (UINT x = 0; x < col - 1; x++)
		{
			indices.push_back(row * z + x);//0
			indices.push_back(row * (z + 1) + x);//1
			indices.push_back(row * z + x + 1);//2

			indices.push_back(row * z + x + 1);//2
			indices.push_back(row * (z + 1) + x);//1
			indices.push_back(row * (z + 1) + x + 1);//3
		}
	}
}
