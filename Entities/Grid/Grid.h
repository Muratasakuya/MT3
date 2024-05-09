#pragma once
#include "MyMath.h"

/// <summary>
/// グリッド線クラス
/// </summary>
class Grid {
private:
	/// <summary>
	/// メンバ変数
	/// </summary>

public:
	/// <summary>
	/// メンバ関数
	/// </summary>

	// コンストラクタ
	Grid() {};
	// デストラクタ
	~Grid() {}

	void DrawGrid(const Matrix4x4& viewMatrix, const Matrix4x4& projectionMatrix, const Matrix4x4& viewportMatrix);
};