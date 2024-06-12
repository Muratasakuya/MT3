#include "OBB.h"

/// <summary>
/// OBBの描画
/// </summary>
/// <param name="obb"></param>
/// <param name="viewProjectionMatrix"></param>
/// <param name="viewportMatrix"></param>
/// <param name="color"></param>
void OBB::DrawOBB(const OBBInfo& obb, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {

	/*AABB aabb;
	aabb.min.x = obb.center.x - obb.size.x;
	aabb.min.y = obb.center.y - obb.size.y;
	aabb.min.z = obb.center.z - obb.size.z;
	aabb.max.x = obb.center.x + obb.size.x;
	aabb.max.y = obb.center.y + obb.size.y;
	aabb.max.z = obb.center.z + obb.size.z;*/
	Vector3 vertices[8];
	Vector3 halfSizeX = obb.orientations[0] * obb.size.x;
	Vector3 halfSizeY = obb.orientations[1] * obb.size.y;
	Vector3 halfSizeZ = obb.orientations[2] * obb.size.z;

	vertices[frontLeftBottom] = obb.center - halfSizeX - halfSizeY - halfSizeZ;
	vertices[frontLeftTop] = obb.center - halfSizeX + halfSizeY - halfSizeZ;
	vertices[frontRightBottom] = obb.center + halfSizeX - halfSizeY - halfSizeZ;
	vertices[frontRightTop] = obb.center + halfSizeX + halfSizeY - halfSizeZ;
	vertices[backLeftBottom] = obb.center - halfSizeX - halfSizeY + halfSizeZ;
	vertices[backLeftTop] = obb.center - halfSizeX + halfSizeY + halfSizeZ;
	vertices[backRightBottom] = obb.center + halfSizeX - halfSizeY + halfSizeZ;
	vertices[backRightTop] = obb.center + halfSizeX + halfSizeY + halfSizeZ;
	Vector3 points[8];
	for (int32_t index = 0; index < 8; ++index) {
		points[index] = Transform(Transform(vertices[index], viewProjectionMatrix), viewportMatrix);
	}

	for (int i = 0; i < 2; ++i) {
		Novice::DrawLine(static_cast<int>(points[frontLeftBottom + i].x), static_cast<int>(points[frontLeftBottom + i].y),
			static_cast<int>(points[frontRightBottom + i].x), static_cast<int>(points[frontRightBottom + i].y), color);
	}
	for (int i = 0; i < 2; ++i) {
		Novice::DrawLine(static_cast<int>(points[backLeftBottom + i].x), static_cast<int>(points[backLeftBottom + i].y),
			static_cast<int>(points[backRightBottom + i].x), static_cast<int>(points[backRightBottom + i].y), color);
	}
	for (int i = 0; i < 4; ++i) {
		Novice::DrawLine(static_cast<int>(points[frontLeftBottom + i].x), static_cast<int>(points[frontLeftBottom + i].y),
			static_cast<int>(points[backLeftBottom + i].x), static_cast<int>(points[backLeftBottom + i].y), color);
	}
	for (int i = 0; i < 8; i += 2) {
		Novice::DrawLine(static_cast<int>(points[frontLeftBottom + i].x), static_cast<int>(points[frontLeftBottom + i].y),
			static_cast<int>(points[frontLeftTop + i].x), static_cast<int>(points[frontLeftTop + i].y), color);
	}
}