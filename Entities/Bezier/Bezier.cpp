#include "Bezier.h"

/// <summary>
/// ベジェ曲線の描画を行う関数
/// </summary>
/// <param name="controlPoint0"></param>
/// <param name="controlPoint1"></param>
/// <param name="controlPoint2"></param>
/// <param name="viewMatrix"></param>
/// <param name="projectionMatrix"></param>
/// <param name="viewportMatrix"></param>
/// <param name="color"></param>
void Bezier::DrawBezier(
	const Vector3& controlPoint0, const Vector3& controlPoint1, const Vector3& controlPoint2,
	const Matrix4x4& viewMatrix, const Matrix4x4& projectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {

	// セグメント数
	const int numSegments = 100;
	std::vector<Vector3> points;

	// ベジェ曲線上の点を計算
	for (int i = 0; i < numSegments; i++) {
		float t = static_cast<float>(i) / numSegments;

		// ベジェ曲線上の点を計算
		Vector3 p0p1 = Vector3Lerp(controlPoint0, controlPoint1, t);
		Vector3 p1p2 = Vector3Lerp(controlPoint1, controlPoint2, t);
		Vector3 p = Vector3Lerp(p0p1, p1p2, t);

		// 座標変換
		Vector3 transformed = Transform(p, viewMatrix);
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