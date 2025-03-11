#pragma once

class CubeMapEditorScene : public Scene
{
public:
	CubeMapEditorScene();
	~CubeMapEditorScene();

	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;

private:
	CubeMapEditor* mapEditor;
};