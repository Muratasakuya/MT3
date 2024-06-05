#include "AABB.h"
#include "Novice.h"
#include <vector>

/// <summary>
/// AABBを描画する関数
/// </summary>
/// <param name="aabb"></param>
/// <param name="viewMatrix"></param>
/// <param name="projectionMatrix"></param>
/// <param name="viewportMatrix"></param>
/// <param name="color"></param>
void AABB::DrawAABB(const AABBInfo& aabb, const Matrix4x4& viewMatrix, const Matrix4x4& projectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {

	// AABBの各頂点
	std::vector<Vector3> vertices = {

		{aabb.min.x, aabb.min.y, aabb.min.z},
		{aabb.max.x, aabb.min.y, aabb.min.z},
		{aabb.min.x, aabb.max.y, aabb.min.z},
		{aabb.max.x, aabb.max.y, aabb.min.z},
		{aabb.min.x, aabb.min.y, aabb.max.z},
		{aabb.max.x, aabb.min.y, aabb.max.z},
		{aabb.min.x, aabb.max.y, aabb.max.z},
		{aabb.max.x, aabb.max.y, aabb.max.z}
	};

	// 各頂点をスクリーン座標に変換
	for (auto& vertex : vertices) {

		vertex = Transform(vertex, viewMatrix);
		vertex = Transform(vertex, projectionMatrix);
		vertex = Transform(vertex, viewportMatrix);
	}

	// 各辺を描画
	std::vector<std::pair<int, int>> edges = {
		{0, 1}, {1, 3}, {3, 2}, {2, 0}, // 前面
		{4, 5}, {5, 7}, {7, 6}, {6, 4}, // 背面
		{0, 4}, {1, 5}, {2, 6}, {3, 7}  // 前面と背面を繋ぐ辺
	};

	for (const auto& edge : edges) {

		const Vector3& start = vertices[edge.first];
		const Vector3& end = vertices[edge.second];

		// 各辺の描画
		Novice::DrawLine(
			static_cast<int>(start.x), static_cast<int>(start.y),
			static_cast<int>(end.x), static_cast<int>(end.y),
			color
		);
	}
}