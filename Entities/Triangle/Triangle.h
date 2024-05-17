#pragma once
#include "MyMath.h"

/// <summary>
/// 三角形クラス
/// </summary>
class Triangle {
private:
	/// <summary>
	/// メンバ変数
	/// </summary>

	/// <summary>
	/// 各頂点の構造体
	/// </summary>
	struct Vertex {

		Matrix4x4 worldMatrix;

		Vector3 ndcPos;
		Vector3 screenPos;
	};

	Vertex vertices_[3]{};

	TriangleInfo info;

public:
	/// <summary>
	/// メンバ関数
	/// </summary>

	// 初期化
	void Initialize(const TriangleInfo triangle);

	// 平面の描画
	void DrawTriangle(const TriangleInfo& triangle,const Matrix4x4& viewMatrix, const Matrix4x4& projectionMatrix, const Matrix4x4& viewportMatrix);
};

