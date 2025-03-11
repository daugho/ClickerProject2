#pragma once

class InstancingScene : public Scene
{
private:
	const float SIZE = 5000;

public:
	InstancingScene();
	~InstancingScene();

	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;

private:
	//vector<Quad*> quads;

	Quad* quad;

	vector<Matrix> instanceData;
	VertexBuffer* instanceBuffer;
};