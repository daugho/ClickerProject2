#pragma once

class SceneManager : public Singleton<SceneManager>
{
private:
    friend class Singleton;

    SceneManager();
    ~SceneManager();

public:
    void Update();

    void PreRender();
    void Render();
    void PostRender();
    void GUIRender();

    void Create(string key, Scene* scene);

    void Add(string key);
    void Remove(string key);

private:
    void AddScene();
    void RemoveScene();

private:
    unordered_map<string, Scene*> scenes;

    list<Scene*> curScenes;

    vector<string> addScenes, removeScenes;
};
