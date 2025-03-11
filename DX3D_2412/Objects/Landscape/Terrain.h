#pragma once

class Terrain : public GameObject<VertexUVNormal>
{
private:
	typedef VertexUVNormal VertexType;
	const float MAX_HEIGHT = 20.0f;

public:
	Terrain();
	~Terrain();

	void Render();

private:
	void MakeMesh();	

private:
	UINT width, height;

	Texture* heightMap;
};
