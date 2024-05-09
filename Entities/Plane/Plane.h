#pragma once
#include "MyMath.h"

/// <summary>
/// 平面クラス
/// </summary>
class Plane{
private:
	/// <summary>
	/// メンバ変数
	/// </summary>

public:
	/// <summary>
	/// メンバ関数
	/// </summary>
	
	// 平面の描画
	void DrawPlane(const PlaneInfo& plane,const Matrix4x4& viewMatrix, const Matrix4x4& projectionMatrix, const Matrix4x4& viewportMatrix);
};