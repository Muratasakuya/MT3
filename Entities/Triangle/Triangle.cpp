#include "Triangle.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="triangle"></param>
void Triangle::Initialize(const TriangleInfo triangle) {

	info = triangle;
}

/// <summary>
/// 三角形を描画する関数
/// </summary>
/// <param name="triangle"></param>
/// <param name="viewMatrix"></param>
/// <param name="projectionMatrix"></param>
/// <param name="viewportMatrix"></param>
void Triangle::DrawTriangle(const TriangleInfo& triangle, const Matrix4x4& viewMatrix,
	const Matrix4x4& projectionMatrix, const Matrix4x4& viewportMatrix) {

	info = triangle;

	for (uint32_t i = 0; i < 3; i++) {

		/// 各頂点の変換
		vertices_[i].worldMatrix
			= MakeAffineMatrix({ 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f }, info.vertices[i]);
		vertices_[i].ndcPos =
			Transform(info.vertices[i], Multiply(viewMatrix, projectionMatrix));
		vertices_[i].screenPos =
			Transform(vertices_[i].ndcPos, viewportMatrix);

		/// 描画
		Novice::DrawTriangle(
			static_cast<int>(vertices_[0].screenPos.x), static_cast<int>(vertices_[0].screenPos.y),
			static_cast<int>(vertices_[1].screenPos.x), static_cast<int>(vertices_[1].screenPos.y),
			static_cast<int>(vertices_[2].screenPos.x), static_cast<int>(vertices_[2].screenPos.y),
			0xffffffff, kFillModeWireFrame
		);
	}
}