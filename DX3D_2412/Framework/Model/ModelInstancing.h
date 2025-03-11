#pragma once

class ModelInstancing : public Model
{
public:
    ModelInstancing(string name, UINT size);
    ~ModelInstancing();

    void Update();
    void Render();
    void Edit();

    Transform* Add();

private:
    vector<Transform*> transforms;
    Matrix* instanceDatas;

    VertexBuffer* instanceBuffer;

    UINT drawCount;
    UINT size;
};