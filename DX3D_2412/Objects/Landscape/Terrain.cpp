#include "Framework.h"

Terrain::Terrain()
{
	material->SetShader(L"Light/Light.hlsl");
	//material->SetDiffuseMap(L"Resources/Textures/Colors/White.png");
	//material->SetDiffuseMap(L"Resources/Textures/Landscape/Dirt2.png");
	material->SetDiffuseMap(L"Resources/Textures/Landscape/Fieldstone_DM.tga");
	material->SetSpecularMap(L"Resources/Textures/Landscape/Fieldstone_SM.tga");
	material->SetNormalMap(L"Resources/Textures/Landscape/Fieldstone_NM.tga");

	heightMap = Texture::Add(L"Resources/Textures/HeightMaps/HeightMap.png");

	MakeMesh();
	MakeNormal();
	mesh->CreateMesh();	
}

Terrain::~Terrain()
{
}

void Terrain::Render()
{
	GameObject::Render();
}

void Terrain::MakeMesh()
{
	width = (UINT)heightMap->GetSize().x;
	height = (UINT)heightMap->GetSize().y;

	vector<Float4> pixels;
	heightMap->ReadPixels(pixels);

	vector<VertexType>& vertices = mesh->GetVertices();
	vertices.reserve(width * height);	

	for (UINT z = 0; z < height; z++)
	{
		for (UINT x = 0; x < width; x++)
		{
			VertexType vertex;
			vertex.pos = {(float)x, 0.0f, (float)z};
			vertex.uv.x = x / (float)(width - 1);
			vertex.uv.y = z / (float)(height - 1);

			UINT index = width * z + x;
			//vertex.pos.y = pixels[index].x * MAX_HEIGHT;

			vertices.push_back(vertex);
		}
	}

	vector<UINT>& indices = mesh->GetIndices();

	indices.reserve((width - 1) * (height - 1) * 6);

	for (UINT z = 0; z < height - 1; z++)
	{
		for (UINT x = 0; x < width - 1; x++)
		{
			indices.push_back(width * z + x);//0
			indices.push_back(width * (z + 1) + x);//1
			indices.push_back(width * z + x + 1);//2

			indices.push_back(width * z + x + 1);//2
			indices.push_back(width * (z + 1) + x);//1
			indices.push_back(width * (z + 1) + x + 1);//3
		}
	}
}
