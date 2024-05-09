#include "MyMath.h"

/*=========================================================================*/
// その他
/*=========================================================================*/

/// πの値の取得
float Pi() { return static_cast<float>(M_PI); }
/// 内積
float Dot(const Vector3& v1, const Vector3& v2) {

	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}
/// 長さ、ノルム
float Length(const Vector3& v) {

	return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}
/*=========================================================================*/
// ベクトル
/*=========================================================================*/

/// 正規化
Vector3 Normalize(const Vector3& v) {

	float length = Length(v);
	if (length != 0) {
		return Vector3(v.x / length, v.y / length, v.z / length);
	}
	else {

		// 値が入ってなければnullで返す
		return Vector3(0.0f, 0.0f, 0.0f);
	}
}
/// クロス積
Vector3 Cross(const Vector3& v1, const Vector3& v2) {

	return Vector3(
		v1.y * v2.z - v1.z * v2.y,
		v1.z * v2.x - v1.x * v2.z,
		v1.x * v2.y - v1.y * v2.x
	);
}
/// 正射影ベクトル
Vector3 Project(const Vector3& v1, const Vector3& v2) {

	Vector3 diff = Normalize(v2);

	float t = Dot(v1, diff);

	Vector3 projectionA = diff * t;

	return projectionA;
}
/// 最近接点
Vector3 ClosestPoint(const Vector3& point, const Segement& segment) {

	Vector3 projection = segment.origin + Project(point - segment.origin, segment.diff);

	// clamp
	float t = std::clamp(Dot(projection - segment.origin, segment.diff) / Dot(segment.diff, segment.diff), 0.0f, 1.0f);

	return segment.origin + segment.diff * t;
}
/*=========================================================================*/
// マトリックス
/*=========================================================================*/

/// 4x4行列の加算
Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2) {

	Matrix4x4 matrix;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			matrix.m[i][j] = m1.m[i][j] + m2.m[i][j];
		}
	}
	return matrix;
}
/// 4x4行列の減算
Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2) {

	Matrix4x4 matrix;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			matrix.m[i][j] = m1.m[i][j] - m2.m[i][j];
		}
	}
	return matrix;
}
/// 4x4行列の積
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {

	Matrix4x4 matrix;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			matrix.m[i][j] = 0;
			for (int k = 0; k < 4; ++k) {
				matrix.m[i][j] += m1.m[i][k] * m2.m[k][j];
			}
		}
	}
	return matrix;
}
/// 4x4行列の逆行列
Matrix4x4 Inverse(const Matrix4x4& m) {
	Matrix4x4 invMatrix = {};

	float det =
		m.m[0][0] * (m.m[1][1] * m.m[2][2] - m.m[1][2] * m.m[2][1]) +
		m.m[0][1] * (m.m[1][2] * m.m[2][0] - m.m[1][0] * m.m[2][2]) +
		m.m[0][2] * (m.m[1][0] * m.m[2][1] - m.m[1][1] * m.m[2][0]);

	if (det == 0) {
		return invMatrix; // ゼロ除算のエラー処理
	}

	float invDet = 1.0f / det;

	invMatrix.m[0][0] = (m.m[1][1] * m.m[2][2] - m.m[1][2] * m.m[2][1]) * invDet;
	invMatrix.m[0][1] = (m.m[0][2] * m.m[2][1] - m.m[0][1] * m.m[2][2]) * invDet;
	invMatrix.m[0][2] = (m.m[0][1] * m.m[1][2] - m.m[0][2] * m.m[1][1]) * invDet;
	invMatrix.m[0][3] = 0.0f;

	invMatrix.m[1][0] = (m.m[1][2] * m.m[2][0] - m.m[1][0] * m.m[2][2]) * invDet;
	invMatrix.m[1][1] = (m.m[0][0] * m.m[2][2] - m.m[0][2] * m.m[2][0]) * invDet;
	invMatrix.m[1][2] = (m.m[0][2] * m.m[1][0] - m.m[0][0] * m.m[1][2]) * invDet;
	invMatrix.m[1][3] = 0.0f;

	invMatrix.m[2][0] = (m.m[1][0] * m.m[2][1] - m.m[1][1] * m.m[2][0]) * invDet;
	invMatrix.m[2][1] = (m.m[0][1] * m.m[2][0] - m.m[0][0] * m.m[2][1]) * invDet;
	invMatrix.m[2][2] = (m.m[0][0] * m.m[1][1] - m.m[0][1] * m.m[1][0]) * invDet;
	invMatrix.m[2][3] = 0.0f;

	invMatrix.m[3][0] = -(m.m[3][0] * invMatrix.m[0][0] + m.m[3][1] * invMatrix.m[1][0] + m.m[3][2] * invMatrix.m[2][0]);
	invMatrix.m[3][1] = -(m.m[3][0] * invMatrix.m[0][1] + m.m[3][1] * invMatrix.m[1][1] + m.m[3][2] * invMatrix.m[2][1]);
	invMatrix.m[3][2] = -(m.m[3][0] * invMatrix.m[0][2] + m.m[3][1] * invMatrix.m[1][2] + m.m[3][2] * invMatrix.m[2][2]);
	invMatrix.m[3][3] = 1.0f;

	return invMatrix;
}
/// 4x4行列の転置行列
Matrix4x4 Transpose(const Matrix4x4& m) {

	Matrix4x4 matrix;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			matrix.m[i][j] = m.m[j][i];
		}
	}

	return matrix;
}
/// 4x4行列の単位行列
Matrix4x4 MakeIdentity4x4() {

	Matrix4x4 matrix;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			matrix.m[i][j] = (i == j) ? 1.0f : 0.0f;
		}
	}

	return matrix;
}
/// 4x4行列の拡縮行列
Matrix4x4 MakeScaleMatrix(const Vector3& scale) {

	Matrix4x4 scaleMatrix = {
		scale.x, 0.0f, 0.0f ,0.0f,
		0.0f, scale.y, 0.0f, 0.0f,
		0.0f, 0.0f, scale.z, 0.0f,
		0.0f ,0.0f, 0.0f, 1.0f
	};

	return scaleMatrix;
}
/// 4x4行列のX軸回転行列
Matrix4x4 MakePitchMatrix(float radian) {

	float cosTheta = std::cos(radian);
	float sinTheta = std::sin(radian);

	Matrix4x4 pitchMatrix = {
		1.0f, 0.0f,0.0f,0.0f,
		0.0f, cosTheta, sinTheta, 0.0f,
		0.0f, -sinTheta, cosTheta, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f };

	return pitchMatrix;
}
/// 4x4行列のY軸回転行列
Matrix4x4 MakeYawMatrix(float radian) {

	float cosTheta = std::cos(radian);
	float sinTheta = std::sin(radian);

	Matrix4x4 yawMatrix = {
		cosTheta, 0.0f, -sinTheta, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		sinTheta, 0.0f, cosTheta, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	return yawMatrix;
}
/// 4x4行列のZ軸回転行列
Matrix4x4 MakeRollMatrix(float radian) {

	float cosTheta = std::cos(radian);
	float sinTheta = std::sin(radian);

	Matrix4x4 rollMatrix = {
		cosTheta, sinTheta, 0.0f, 0.0f,
		-sinTheta, cosTheta, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f,0.0f,0.0f, 1.0f
	};

	return rollMatrix;
}
/// 4x4行列の回転行列
Matrix4x4 MakeRotateMatrix(const Vector3& rotate) {

	Matrix4x4 pitchMatrix = MakePitchMatrix(rotate.x);	// X軸回転行列
	Matrix4x4 yawMatrix = MakeYawMatrix(rotate.y);		// Y軸回転行列
	Matrix4x4 rollMatrix = MakeRollMatrix(rotate.z);	// Z軸回転行列

	Matrix4x4 rotateMatrix = Multiply(pitchMatrix, Multiply(yawMatrix, rollMatrix));

	return rotateMatrix;
}
/// 4x4行列の平行移動行列
Matrix4x4 MakeTranslateMatrix(const Vector3& translate) {

	Matrix4x4 translateMatrix = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		translate.x, translate.y, translate.z, 1.0f
	};

	return translateMatrix;
}
/// 4x4行列のアフィン変換
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {

	Matrix4x4 matrix = {};

	Matrix4x4 scaleMatrix = MakeScaleMatrix(scale);
	Matrix4x4 rotateMatrix = MakeRotateMatrix(rotate);
	Matrix4x4 translateMatrix = MakeTranslateMatrix(translate);

	matrix = Multiply(scaleMatrix, rotateMatrix);
	matrix = Multiply(matrix, translateMatrix);

	return matrix;
}
/// 4x4行列の座標変換
Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix) {

	Vector3 result;

	// ベクトルと行列の乗算
	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] +
		matrix.m[3][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] +
		matrix.m[3][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] +
		matrix.m[3][2];
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] +
		matrix.m[3][3];

	// ベクトルの正規化
	if (w != 0.0f) {
		result.x /= w;
		result.y /= w;
		result.z /= w;
	}

	return result;
}
