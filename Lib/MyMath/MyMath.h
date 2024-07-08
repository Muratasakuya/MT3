#pragma once
#include <Novice.h>
#include <cmath>
#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>
#include <stdint.h>
#include <imgui.h>

#include "Vector.h"
#include "Matrix.h"

/*=========================================================================*/
// その他
/*=========================================================================*/

/// lineの種類
enum class LineType {

	Ray,        // 半直線
	LineSegment // 線分
};

/// 三角形
struct TriangleInfo {

	Vector3 vertices[3];
};

/// 平面
struct PlaneInfo {

	Vector3 normal;
	float distance;
};

/// 球
struct SphereInfo {

	float radius;
	Vector3 center;
	Vector3 rotate;
	uint32_t color;
};

/// 線分
struct Segment {

	Vector3 origin; // 始点
	Vector3 diff;   // 終点への差分ベクトル
};

/// 線
struct LineInfo {

	Vector3 origin; // 始点
	Vector3 diff;   // 終点への差分ベクトル

	LineType type;

	uint32_t color;
};

struct AABBInfo {

	Vector3 min;
	Vector3 max;
};

struct Spring {

	Vector3 anchor;           // アンカー。固定された端の位置
	float naturalLength;      // 自然長
	float stiffness;          // 剛性。バネ定数k
	float dampingCoefficient; // 減衰係数
};

struct Ball {

	Vector3 pos;
	Vector3 velocity;
	Vector3 acceleration;
	float mass;
	float radius;
	uint32_t color;
};

/// πの値の取得
float Pi();

/// 内積
float Dot(const Vector3& v1, const Vector3& v2);

/// 長さ、ノルム
float Length(const Vector3& v);

/// 線形補間関数
float Lerp(float a, float b, float t);

/*=========================================================================*/
// ベクトル
/*=========================================================================*/

/// Vector3線形補間関数
Vector3 Vector3Lerp(const Vector3& a, const Vector3& b, float t);

/// 正規化
Vector3 Normalize(const Vector3& v);

/// クロス積
Vector3 Cross(const Vector3& v1, const Vector3& v2);

/// 正射影ベクトル
Vector3 Project(const Vector3& v1, const Vector3& v2);

/// 最近接点
Vector3 ClosestPoint(const Vector3& point, const Segment& segment);

// 法線と垂直なベクトル
Vector3 Perpendicular(const Vector3& v);

/*=========================================================================*/
// マトリックス
/*=========================================================================*/

/// 4x4行列の加算
Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2);

/// 4x4行列の減算
Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2);

/// 4x4行列の積
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);

/// 4x4行列の逆行列
Matrix4x4 Inverse(const Matrix4x4& m);

/// 4x4行列の転置行列
Matrix4x4 Transpose(const Matrix4x4& m);

/// 4x4行列の単位行列
Matrix4x4 MakeIdentity4x4();

/// 4x4行列の拡縮行列
Matrix4x4 MakeScaleMatrix(const Vector3& scale);

/// 4x4行列のX軸回転行列
Matrix4x4 MakePitchMatrix(float radian);

/// 4x4行列のY軸回転行列
Matrix4x4 MakeYawMatrix(float radian);

/// 4x4行列のZ軸回転行列
Matrix4x4 MakeRollMatrix(float radian);

/// 4x4行列の回転行列
Matrix4x4 MakeRotateMatrix(const Vector3& rotate);

/// 4x4行列の平行移動行列
Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

/// 4x4行列のアフィン変換
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

/// 4x4行列の座標変換
Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);