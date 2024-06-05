#pragma once
#include <Novice.h>
#include "MyMath.h"

/// <summary>
/// AABBクラス
/// Axis Aligned Bounding Box
/// </summary>
class AABB {
private:
	/// <summary>
	/// メンバ変数
	/// </summary>



public:
	/// <summary>
	/// メンバ関数
	/// </summary>

	// AABBの描画
	void DrawAABB(const AABBInfo& aabb, const Matrix4x4& viewMatrix, const Matrix4x4& projectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);
};