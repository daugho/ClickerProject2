#include "Framework.h"

Sphere::Sphere(float radius, UINT sliceCount, UINT stackCount)
	: radius(radius), sliceCount(sliceCount), stackCount(stackCount)
{
	SetTag("Sphere");

	MakeMesh();
    MakeTangent();
	mesh->CreateMesh();
}

void Sphere::MakeMesh()
{
    float thetaStep = XM_2PI / sliceCount;
    float phiStep = XM_PI / stackCount;

    vector<VertexType>& vertices = mesh->GetVertices();

    vertices.reserve((sliceCount + 1) * (stackCount + 1));

    for (UINT i = 0; i <= stackCount; i++)
    {
        float phi = i * phiStep;

        for (int j = 0; j <= sliceCount; j++)
        {
            float theta = j * thetaStep;

            VertexType vertex;

            vertex.normal.x = sin(phi) * cos(theta);
            vertex.normal.y = cos(phi);
            vertex.normal.z = sin(phi) * sin(theta);

            vertex.pos = Vector3(vertex.normal) * radius;

            vertex.uv.x = (float)j / sliceCount;
            vertex.uv.y = (float)i / stackCount;

            vertices.push_back(vertex);
        }
    }

    vector<UINT>& indices = mesh->GetIndices();

    indices.reserve(sliceCount * stackCount * 6);

    for (UINT z = 0; z < stackCount; z++)
    {
        for (UINT x = 0; x < sliceCount; x++)
        {
            indices.push_back((sliceCount + 1) * z + x);//0
            indices.push_back((sliceCount + 1) * z + x + 1);//3
            indices.push_back((sliceCount + 1) * (z + 1) + x);//1

            indices.push_back((sliceCount + 1) * (z + 1) + x);//1            
            indices.push_back((sliceCount + 1) * z + x + 1);//3
            indices.push_back((sliceCount + 1) * (z + 1) + x + 1);//2
        }
    }
}
