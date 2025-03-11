#include "Framework.h"
#include "GameMath.h"

int GameMath::Random(const int& min, const int& max)
{
    return rand() % (max - min) + min;
}

float GameMath::Random(const float& min, const float& max)
{
    float normal = (float)rand() / (float)RAND_MAX;//0~1

    return min + (max - min) * normal;
}

Vector3 GameMath::Random(const Vector3& min, const Vector3& max)
{
    return Vector3(Random(min.x, max.x), Random(min.y, max.y), Random(min.z, max.z));
}

float GameMath::Clamp(const float& min, const float& max, const float& value)
{
    if (value < min)
        return min;

    if(value > max)
        return max;

    return value;
}

Vector3 GameMath::ClosestPointOnLine(const Vector3& start, const Vector3& end, const Vector3& point)
{
    Vector3 line = end - start;
    Vector3 A = point - start;

    float x = Vector3::Dot(line, A);
    float y = Vector3::Dot(line, line);

    float t = Clamp(0, 1, x / y);

    return start + line * t;
}

//float GameMath::Lerp(const float& start, const float& end, const float& t)
//{
//    return start + (end - start) * t;
//}
//
//Vector2 GameMath::Lerp(const Vector2& start, const Vector2& end, const float& t)
//{
//    return start + (end - start) * t;
//}
//
//Vector3 GameMath::Lerp(const Vector3& start, const Vector3& end, const float& t)
//{
//    return start + (end - start) * t;
//}

