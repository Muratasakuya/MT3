#include "Sphere.h"

/// <summary>
/// 球を描画する関数
/// </summary>
/// <summary>
/// 球を描画する関数
/// </summary>
void Sphere::DrawSphere(const Matrix4x4& worldMatrix, float radius, uint32_t color, const Matrix4x4& viewMatrix, const Matrix4x4& projectionMatrix, const Matrix4x4& viewportMatrix) {

	// 分割数
	const uint32_t kSubdivision = 12;
	// 緯度分割1つ分の角度
	const float kLatEvery = Pi() / kSubdivision;
	// 経度分割1つ分の角度
	const float kLonEvery = 2.0f * Pi() / kSubdivision;

	// World-View-Projection 行列の計算
	Matrix4x4 wvpMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));

	auto calculatePoint = [&](float lat, float lon) -> Vector3 {
		return {
			radius* std::cos(lat) * std::cos(lon),
			radius* std::sin(lat),
			radius* std::cos(lat) * std::sin(lon)
		};
		};

	auto transformToScreen = [&](const Vector3& point) -> Vector3 {
		Vector3 ndc = Transform(point, wvpMatrix);
		return Transform(ndc, viewportMatrix);
		};

	// 緯度方向に分割 -π/2 ~ π/2
	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
		// 現在の緯度
		float lat = -Pi() / 2.0f + kLatEvery * latIndex;

		// 経度の方向に分割 0 ~ 2π
		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
			// 現在の経度
			float lon = lonIndex * kLonEvery;

			// world座標系でのa、b、cを求める
			Vector3 pointA = calculatePoint(lat, lon);
			Vector3 pointB = calculatePoint(lat + kLatEvery, lon);
			Vector3 pointC = calculatePoint(lat, lon + kLonEvery);

			// スクリーン座標に変換
			Vector3 screenPointA = transformToScreen(pointA);
			Vector3 screenPointB = transformToScreen(pointB);
			Vector3 screenPointC = transformToScreen(pointC);

			// Draw lines
			Novice::DrawLine(
				static_cast<int>(screenPointA.x), static_cast<int>(screenPointA.y),
				static_cast<int>(screenPointB.x), static_cast<int>(screenPointB.y),
				color
			);

			Novice::DrawLine(
				static_cast<int>(screenPointA.x), static_cast<int>(screenPointA.y),
				static_cast<int>(screenPointC.x), static_cast<int>(screenPointC.y),
				color
			);
		}
	}
}