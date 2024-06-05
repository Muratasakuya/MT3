#pragma once
#include <Novice.h>
#include <vector>
#include "MyMath.h"

/// <summary>
/// Bezierクラス
/// </summary>
class Bezier {
private:
	/// <summary>
	/// メンバ変数
	/// </summary>



public:
	/// <summary>
	/// メンバ関数
	/// </summary>

	// ベジェ曲線の描画
	void DrawBezier(
		const Vector3& controlPoint0, const Vector3& controlPoint1, const Vector3& controlPoint2,
		const Matrix4x4& viewMatrix, const Matrix4x4& projectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);
};