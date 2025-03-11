#pragma once

class PacmanScene : public Scene
{
public:
	PacmanScene();
	~PacmanScene();

	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;
};