#pragma once

class GridScene : public Scene
{
private:
	const int MAX_SIZE = 100;

public:
	GridScene();
	~GridScene();

	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;

private:
	void MakeMesh();

private:
	Material* material;
	MatrixBuffer* worldBuffer;
	Mesh<VertexColor>* mesh;
};