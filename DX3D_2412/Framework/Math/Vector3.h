#pragma once

class Vector3
{
public:
	Vector3(float x = 0.0f, float y = 0.0f, float z = 0.0f)
		: value(XMVectorSet(x, y, z, 0))
	{}
	
	Vector3(Float3 value) : value(XMLoadFloat3(&value))		
	{}

	Vector3(Vector4 value) : value(value)
	{}

	operator Vector4() { return value; }
	operator Float3()
	{
		Float3 result;
		XMStoreFloat3(&result, value);

		return result;
	}
	Vector4* Get() { return &value; }

	float GetX() const { return XMVectorGetX(value); }
	float GetY() const { return XMVectorGetY(value); }
	float GetZ() const { return XMVectorGetZ(value); }

	void SetX(const float& v) { value = XMVectorSetX(value, v); }
	void SetY(const float& v) { value = XMVectorSetY(value, v); }
	void SetZ(const float& v) { value = XMVectorSetZ(value, v); }

	_declspec(property(get = GetX, put = SetX)) float x;
	_declspec(property(get = GetY, put = SetY)) float y;
	_declspec(property(get = GetZ, put = SetZ)) float z;

	float operator[](const UINT& index) const
	{
		switch (index)
		{
		case 0:
			return GetX();
		case 1:
			return GetY();
		case 2:
			return GetZ();
		}
	}

	Vector3 operator+(const Vector3& vector) const { return value + vector.value; }
	Vector3 operator-(const Vector3& vector) const { return value - vector.value; }
	Vector3 operator*(const Vector3& vector) const { return value * vector.value; }
	Vector3 operator/(const Vector3& vector) const { return value / vector.value; }

	void operator+=(const Vector3& vector) { value += vector.value; }
	void operator-=(const Vector3& vector) { value -= vector.value; }
	void operator*=(const Vector3& vector) { value *= vector.value; }
	void operator/=(const Vector3& vector) { value /= vector.value; }

	Vector3 operator+(const float& scala) const { return value + XMVectorReplicate(scala); }
	Vector3 operator-(const float& scala) const { return value - XMVectorReplicate(scala); }
	Vector3 operator*(const float& scala) const { return value * XMVectorReplicate(scala); }
	Vector3 operator/(const float& scala) const { return value / XMVectorReplicate(scala); }

	void operator+=(const float& scala) { value += XMVectorReplicate(scala); }
	void operator-=(const float& scala) { value -= XMVectorReplicate(scala); }
	void operator*=(const float& scala) { value *= XMVectorReplicate(scala); }
	void operator/=(const float& scala) { value /= XMVectorReplicate(scala); }
	bool operator==(const Vector3& other) const	{return x == other.x && y == other.y && z == other.z;
	}
	bool operator!=(const Vector3& other) const { return !(*this == other); };

	Vector3 operator* (const Matrix& matrix) const { return XMVector3TransformCoord(value, matrix);	}
	void operator*= (const Matrix& matrix)	{ value = XMVector3TransformCoord(value, matrix); }

	friend void operator+=(Float3& v1, const Vector3& v2)
	{
		Vector4 temp = XMLoadFloat3(&v1) + v2.value;
		XMStoreFloat3(&v1, temp);
	}
	friend void operator-=(Float3& v1, const Vector3& v2)
	{
		Vector4 temp = XMLoadFloat3(&v1) - v2.value;
		XMStoreFloat3(&v1, temp);
	}

	Vector3 GetNormalized() const { return XMVector3Normalize(value); }
	void Normalize() { value = XMVector3Normalize(value); }

	float Magnitude() const { return XMVectorGetX(XMVector3Length(value)); }

public:
	static Vector3 Zero() { return Vector3(); }
	static Vector3 One() { return Vector3(1, 1, 1); }
	static Vector3 Right() { return Vector3(1, 0, 0); }
	static Vector3 Left() { return Vector3(-1, 0, 0); }
	static Vector3 Up() { return Vector3(0, 1, 0); }
	static Vector3 Down() { return Vector3(0, -1, 0); }
	static Vector3 Forward() { return Vector3(0, 0, 1); }
	static Vector3 Back() { return Vector3(0, 0, -1); }

	static Vector3 Cross(const Vector3& v1, const Vector3& v2)
	{
		return XMVector3Cross(v1.value, v2.value);
	}
	static float Dot(const Vector3& v1, const Vector3& v2)
	{
		return XMVectorGetX(XMVector3Dot(v1.value, v2.value));
	}
	static float Distance(const Vector3& v1, const Vector3& v2)
	{
		return (v2 - v1).Magnitude();
	}

private:
	Vector4 value;
};