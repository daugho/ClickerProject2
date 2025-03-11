#pragma once

struct Vector2
{
    float x, y;

    Vector2(float x = 0.0f, float y = 0.0f)
        : x(x), y(y)
    {
    }
    Vector2(Float2 value) : x(value.x), y(value.y)
    {
    }
    Vector2(class Vector3 value) : x(value.x), y(value.y)
    {
    }
    Vector2(POINT value) : x(value.x), y(value.y)
    {
    }

    Vector2 operator+(const Vector2& value) const
    {
        return Vector2(x + value.x, y + value.y);
    }
    Vector2 operator-(const Vector2& value) const
    {
        return Vector2(x - value.x, y - value.y);
    }
    Vector2 operator*(const Vector2& value) const
    {
        return Vector2(x * value.x, y * value.y);
    }
    Vector2 operator/(const Vector2& value) const
    {
        return Vector2(x / value.x, y / value.y);
    }
    Vector2 operator*(const float& value) const
    {
        return Vector2(x * value, y * value);
    }
    Vector2 operator/(const float& value) const
    {
        return Vector2(x / value, y / value);
    }

    void operator+=(const Vector2& value)
    {
        x += value.x;
        y += value.y;
    }
    void operator-=(const Vector2& value)
    {
        x -= value.x;
        y -= value.y;
    }
    void operator*=(const float& value)
    {
        x *= value;
        y *= value;
    }
    void operator/=(const float& value)
    {
        x /= value;
        y /= value;
    }

    Vector2 operator* (const Matrix& value) const
    {
        Float2 coord(x, y);
        XMVECTOR temp = XMLoadFloat2(&coord);

        //XMVector2TransformNormal : w-> 0
        //XMVector2TransformCoord : w-> 1
        temp = XMVector2TransformCoord(temp, value);

        XMStoreFloat2(&coord, temp);

        return coord;
    }

    void operator*= (const Matrix& value)
    {
        Float2 coord(x, y);
        XMVECTOR temp = XMLoadFloat2(&coord);

        temp = XMVector2TransformCoord(temp, value);

        XMStoreFloat2(&coord, temp);

        x = coord.x;
        y = coord.y;
    }

    float Magnitude() const
    {
        return sqrt(x * x + y * y);
    }

    float SqrMagnitude() const
    {
        return x * x + y * y;
    }

    void Normalize()
    {
        float length = Magnitude();

        x /= length;
        y /= length;
    }

    Vector2 Normalized() const
    {
        float length = Magnitude();

        return Vector2(x / length, y / length);
    }

    float Angle() const
    {
        return atan2(-y, x);
    }

    static Vector2 Right() { return Vector2(1, 0); }
    static Vector2 Left() { return Vector2(-1, 0); }
    static Vector2 Up() { return Vector2(0, -1); }
    static Vector2 Down() { return Vector2(0, 1); }

    static float Dot(const Vector2& vec1, const Vector2& vec2)
    {
        return vec1.x * vec2.x + vec1.y * vec2.y;
    }

    static float Distance(const Vector2& vec1, const Vector2& vec2)
    {
        return (vec1 - vec2).Magnitude();
    }
};