#include "Plane.h"

/// <summary>
/// 平面の描画
/// </summary>
/// <param name="plane"></param>
/// <param name="viewProjectionMatrix"></param>
/// <param name="viewportMatrix"></param>
void Plane::DrawPlane(const PlaneInfo& plane, const Matrix4x4& viewMatrix, const Matrix4x4& projectionMatrix, const Matrix4x4& viewportMatrix) {

	// 中心点
	Vector3 center = plane.distance * plane.normal;

	Vector3 perpendiculars[4];

	perpendiculars[0] = Normalize(Perpendicular(plane.normal));
	perpendiculars[1] = { -perpendiculars[0].x,-perpendiculars[0].y,-perpendiculars[0].z };
	perpendiculars[2] = Cross(plane.normal, perpendiculars[0]);
	perpendiculars[3] = { -perpendiculars[2].x,-perpendiculars[2].y,-perpendiculars[2].z };

	Vector3 points[4];
	for (int32_t index = 0; index < 4; index++) {

		Vector3 extend = 2.0f * perpendiculars[index];
		Vector3 point = center + extend;
		points[index] = Transform(Transform(point, Multiply(viewMatrix, projectionMatrix)), viewportMatrix);
	}

	// 左0 右1 奥2 手前 3

	Novice::DrawTriangle(
		static_cast<int>(points[2].x), static_cast<int>(points[2].y),
		static_cast<int>(points[1].x), static_cast<int>(points[1].y),
		static_cast<int>(points[3].x), static_cast<int>(points[3].y),
		0xffffffff, kFillModeSolid
	);
	Novice::DrawTriangle(
		static_cast<int>(points[2].x), static_cast<int>(points[2].y),
		static_cast<int>(points[0].x), static_cast<int>(points[0].y),
		static_cast<int>(points[3].x), static_cast<int>(points[3].y),
		0xffffffff, kFillModeSolid
	);

	// pointsを結んで描画
	Novice::DrawLine(
		static_cast<int>(points[1].x), static_cast<int>(points[1].y),
		static_cast<int>(points[2].x), static_cast<int>(points[2].y),
		0xffffffff
	);
	Novice::DrawLine(
		static_cast<int>(points[2].x), static_cast<int>(points[2].y),
		static_cast<int>(points[0].x), static_cast<int>(points[0].y),
		0xffffffff
	);
	Novice::DrawLine(
		static_cast<int>(points[0].x), static_cast<int>(points[0].y),
		static_cast<int>(points[3].x), static_cast<int>(points[3].y),
		0xffffffff
	);
	Novice::DrawLine(
		static_cast<int>(points[3].x), static_cast<int>(points[3].y),
		static_cast<int>(points[1].x), static_cast<int>(points[1].y),
		0xffffffff
	);
}
