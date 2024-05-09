#pragma once

/// <summary>
/// 二次元ベクトル
/// </summary>
struct Vector2 final {

	float x;
	float y;

	// 算術演算子
	Vector2 operator+(const Vector2& other) const {
		return { x + other.x, y + other.y };
	}
	Vector2 operator-(const Vector2& other) const {
		return { x - other.x, y - other.y };
	}
	Vector2& operator+=(const Vector2& v) {
		x += v.x;
		y += v.y;
		return *this;
	}
	Vector2& operator-=(const Vector2& v) {
		x += v.x;
		y += v.y;
		return *this;
	}
};

/// <summary>
/// 三次元ベクトル
/// </summary>
struct Vector3 final {

	float x;
	float y;
	float z;

	// 算術演算子
	Vector3 operator+(const Vector3& other) const {
		return { x + other.x, y + other.y, z + other.z };
	}

	Vector3 operator-(const Vector3& other) const {
		return { x - other.x, y - other.y, z - other.z };
	}
	Vector3 operator*(float scalar) const {
		return Vector3(x * scalar, y * scalar, z * scalar);
	}
	friend Vector3 operator*(float scalar, const Vector3& v) {
		return Vector3(v.x * scalar, v.y * scalar, v.z * scalar);
	}
	Vector3& operator+=(const Vector3& v) {
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}
	Vector3& operator-=(const Vector3& v) {
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}
};

/// <summary>
/// 四次元ベクトル
/// </summary>
struct Vector4 final {

	float x;
	float y;
	float z;
	float w;
};