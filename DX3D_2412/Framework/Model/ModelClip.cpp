#include "Framework.h"

ModelClip::~ModelClip()
{
    for (pair<string, KeyFrame*> keyFrame : keyFrames)
        delete keyFrame.second;
}

void ModelClip::Init()
{
    duration = frameCount / tickPerSecond;
    eventIter = events.begin();
    playTime = 0.0f;
}

void ModelClip::Excute()
{
    if (events.empty()) return;
    if (eventIter == events.end()) return;

    float ratio = playTime / duration;

    if (eventIter->first > ratio) return;

    eventIter->second();
    eventIter++;
}
