#include "CatmullRom.h"

/// <summary>
/// スプライン曲線の描画を行う関数
/// </summary>
/// <param name="controlPoint0"></param>
/// <param name="controlPoint1"></param>
/// <param name="controlPoint2"></param>
/// <param name="controlPoint3"></param>
/// <param name="viewMatrix"></param>
/// <param name="projectionMatrix"></param>
/// <param name="viewportMatrix"></param>
/// <param name="color"></param>
void CatmullRom::DrawCatmullRom(
	const Vector3& controlPoint0, const Vector3& controlPoint1, const Vector3& controlPoint2, const Vector3& controlPoint3,
	const Matrix4x4& viewMatrix, const Matrix4x4& projectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {

	// セグメント数
	const int numSegments = 100;
	std::vector<Vector3> points;

	// Catmull-Romスプライン上の点を計算
	for (int i = 0; i < numSegments; i++) {
		float t = static_cast<float>(i) / numSegments;

		// Catmull-Romスプライン上の点を計算
		float t2 = t * t;
		float t3 = t2 * t;

		Vector3 point = 0.5f * (
			(2.0f * controlPoint1) +
			(Vector3{-controlPoint0.x,-controlPoint0.y,-controlPoint0.z } + controlPoint2) * t +
			(2.0f * controlPoint0 - 5.0f * controlPoint1 + 4.0f * controlPoint2 - controlPoint3) * t2 +
			(Vector3{ -controlPoint0.x,-controlPoint0.y,-controlPoint0.z } + 3.0f * controlPoint1 - 3.0f
				* controlPoint2 + controlPoint3) * t3);

		// 座標変換
		Vector3 transformed = Transform(point, viewMatrix);
		transformed = Transform(transformed, projectionMatrix);
		transformed = Transform(transformed, viewportMatrix);

		points.push_back(transformed);
	}

	// 点を線で結んで描画
	for (size_t i = 0; i < points.size() - 1; ++i) {
		Novice::DrawLine(
			static_cast<int>(points[i].x), static_cast<int>(points[i].y),
			static_cast<int>(points[i + 1].x), static_cast<int>(points[i + 1].y),
			color
		);
	}
}