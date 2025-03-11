#pragma once

class SolaSystemScene : public Scene
{
public:
	SolaSystemScene();
	~SolaSystemScene();
		
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;

private:	
	vector<Planet*> planets;
};
