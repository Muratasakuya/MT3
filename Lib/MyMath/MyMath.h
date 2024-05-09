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

/// 球
struct SphereInfo {

	float radius_;
	Vector3 center;
	uint32_t color;
};

/// 線分
struct Segement {

	Vector3 origin; // 始点
	Vector3 diff;   // 終点への差分ベクトル
};

/// πの値の取得
float Pi();

/// 内積
float Dot(const Vector3& v1, const Vector3& v2);

/// 長さ、ノルム
float Length(const Vector3& v);

/*=========================================================================*/
// ベクトル
/*=========================================================================*/

/// 正規化
Vector3 Normalize(const Vector3& v);

/// クロス積
Vector3 Cross(const Vector3& v1, const Vector3& v2);

/// 正射影ベクトル
Vector3 Project(const Vector3& v1, const Vector3& v2);

/// 最近接点
Vector3 ClosestPoint(const Vector3& point, const Segement& segment);

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