#pragma once

class CeilingManager
{
public:
    CeilingManager(Vector3 position, int width, int height);
    ~CeilingManager();

    void GenerateCeiling();
    void Render();

private:
    Vector3 ceilingPos;
    vector<Cube*> ceilingCubes;

};