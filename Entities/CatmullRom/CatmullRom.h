#pragma once
#include <Novice.h>
#include "MyMath.h"
#include <vector>

/// <summary>
/// CatmullRomクラス
/// </summary>
class CatmullRom {
public:

	// スプライン曲線の描画
	void DrawCatmullRom(
		const Vector3& controlPoint0, const Vector3& controlPoint1, const Vector3& controlPoint2, const Vector3& controlPoint3,
		const Matrix4x4& viewMatrix, const Matrix4x4& projectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);

};