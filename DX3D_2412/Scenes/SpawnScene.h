#pragma once

class SpawnScene : public Scene
{
private:
	typedef VertexUV VertexType;

	const UINT SIZE = 100;
	const float SPAWN_INTERVAL = 1.0f;
	const Vector3 MIN_POS = { -50.0f, 0.0f, -50.0f };
	const Vector3 MAX_POS = { +50.0f, 0.0f, +50.0f };

public:
	SpawnScene();
	~SpawnScene();

	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;

private:
	void Spawn();
	void LoadTextures();
	void LoadMeshes();

	void MakeCubeMesh();
	void MakeSphereMesh();

private:
	vector<GameObject<VertexType>*> objects;

	float time = 0.0f;

	vector<Texture*> textures;
	vector<Mesh<VertexType>*> meshes;

	Plane* plane;
};