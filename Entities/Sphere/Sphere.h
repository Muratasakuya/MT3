#pragma once
#include "MyMath.h"

/// <summary>
/// グリッド球クラス
/// </summary>
class Sphere {
private:
	/// <summary>
	/// メンバ変数
	/// </summary>

	/// <summary>
	/// 各点の構造体
	/// </summary>
	struct Point {

		Matrix4x4 worldMatrix_;
		Matrix4x4 worldViewProjectionMatrix_;

		Vector3 localPos_;
		Vector3 screenPos_;
	};

	Point a{}, b{}, c{};

	Vector3 ndcPosA_{};
	Vector3 ndcPosB_{};
	Vector3 ndcPosC_{};

public:
	/// <summary>
	/// メンバ関数
	/// </summary>

	// コンストラクタ
	Sphere() {}
	// デストラクタ
	~Sphere() {}

	// 球を描画する関数
	void DrawSphere(SphereInfo& sphere, const Matrix4x4& viewMatrix, const Matrix4x4& projectionMatrix, const Matrix4x4& viewportMatrix);
};