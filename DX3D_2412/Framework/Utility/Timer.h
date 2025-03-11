#pragma once

class Timer : public Singleton<Timer>
{
private:
    friend class Singleton;

    Timer();
    ~Timer();

public:
    void Update();    

    float GetElapsedTime() { return elapsedTime > 0.1f ? 0.1f : elapsedTime * scale; }
    int GetFPS() { return frameRate; }

    void SetScale(float scale) { this->scale = scale; }

private:
    float timeScale = 0.0f;
    float elapsedTime = 0.0f;
    float oneSecCount = 0.0f;
    float scale = 1.0f;
    
    __int64 curTime = 0;
    __int64 lastTime = 0;
    __int64 periodFrquency = 0;

    int frameRate = 0;
    int frameCount = 0;
};