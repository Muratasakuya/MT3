#define NOMINMAX

#include "Collision.h"

/// <summary>
/// 交点計算
/// </summary>
/// <param name="intersection"></param>
/// <param name="p0"></param>
/// <param name="p1"></param>
/// <param name="normal"></param>
/// <param name="pointOnPlane"></param>
/// <returns></returns>
bool LinePlaneIntersection(Vector3& intersection, const Vector3& p0, const Vector3& p1, const Vector3& normal, const Vector3& pointOnPlane) {
	Vector3 lineDir = p1 - p0;
	float d = Dot(normal, pointOnPlane);
	float t = (d - Dot(normal, p0)) / Dot(normal, lineDir);
	if (t >= 0.0f && t <= 1.0f) {
		intersection = { p0.x + t * lineDir.x, p0.y + t * lineDir.y, p0.z + t * lineDir.z };
		return true;
	}
	return false;
}

/// <summary>
/// 球と球の衝突判定
/// </summary>
/// <param name="s1"></param>
/// <param name="s2"></param>
/// <returns></returns>
bool Collision::Sphere2SphereCheckCollision(const SphereInfo& s1, const SphereInfo& s2) {

	float distance = Length(s1.center - s2.center);

	// 半径の合計より大きければ衝突
	if (distance <= s1.radius + s2.radius) {

		return true;
	} else {

		return false;
	}
}

/// <summary>
/// 球と平面の当たり判定
/// </summary>
/// <param name="shere"></param>
/// <param name="plane"></param>
/// <returns></returns>
bool Collision::Sphere2PlaneCheckCollision(const SphereInfo& sphere, const PlaneInfo& plane) {

	float distance = Dot(sphere.center, plane.normal) - plane.distance;

	float distanceFromPlane = fabs(distance);

	// 球の中心が平面からの距離が半径以下なら衝突
	if (distanceFromPlane <= sphere.radius) {

		return true;
	} else {

		return false;
	}
}

/// <summary>
/// 平面と線の当たり判定
/// </summary>
/// <param name="plane"></param>
/// <param name="segment"></param>
/// <returns></returns>
bool Collision::Plane2LineCheckCollision(const PlaneInfo& plane, const LineInfo& line) {

	float dot = Dot(plane.normal, line.diff - line.origin);

	float t = (plane.distance - Dot(plane.normal, line.origin)) / dot;

	// 線分
	if (t >= 0.0f && t <= 1.0f && line.type == LineType::LineSegment) {

		return true;
	}

	return false;
}

/// <summary>
/// 三角形と線の当たり判定
/// </summary>
/// <param name="triangle"></param>
/// <param name="line"></param>
/// <returns></returns>
bool Collision::Triangle2LineCheckCollision(const TriangleInfo& triangle, const LineInfo& line) {

	Vector3 v01 = triangle.vertices[1] - triangle.vertices[0];
	Vector3 v12 = triangle.vertices[2] - triangle.vertices[1];
	Vector3 v20 = triangle.vertices[0] - triangle.vertices[2];

	Vector3 normal = Cross(v01, v12);

	Vector3 intersection;
	if (!LinePlaneIntersection(intersection, line.origin, line.diff, normal, triangle.vertices[0])) {
		return false;
	}

	Vector3 v0p = intersection - triangle.vertices[0];
	Vector3 v1p = intersection - triangle.vertices[1];
	Vector3 v2p = intersection - triangle.vertices[2];

	Vector3 cross01 = Cross(v01, v1p);
	Vector3 cross12 = Cross(v12, v2p);
	Vector3 cross20 = Cross(v20, v0p);

	if (Dot(cross01, normal) >= 0.0f &&
		Dot(cross12, normal) >= 0.0f &&
		Dot(cross20, normal) >= 0.0f) {
		return true;
	}

	return false;
}

/// <summary>
/// AABB同士の当たり判定
/// </summary>
/// <param name="aabb1"></param>
/// <param name="aabb2"></param>
/// <returns></returns>
bool Collision::AABB2AABBCheckCollision(const AABBInfo& aabb1, const AABBInfo& aabb2) {

	if ((aabb1.min.x <= aabb2.max.x && aabb1.max.x >= aabb2.min.x) &&
		(aabb1.min.y <= aabb2.max.y && aabb1.max.y >= aabb2.min.y) &&
		(aabb1.min.z <= aabb2.max.z && aabb1.max.z >= aabb2.min.z)) {

		return true;
	}

	return false;
}

/// <summary>
/// AABBと球の当たり判定
/// </summary>
/// <param name="aabb1"></param>
/// <param name="aabb2"></param>
/// <returns></returns>
bool Collision::AABB2SphereCheckCollision(const AABBInfo& aabb, const SphereInfo& sphere) {

	// 最近接点を求める
	Vector3 closestPoint{
		std::clamp(sphere.center.x,aabb.min.x,aabb.max.x),
		std::clamp(sphere.center.y,aabb.min.y,aabb.max.y),
		std::clamp(sphere.center.z,aabb.min.z,aabb.max.z),
	};

	// 最近接点と球の中心との距離を求める
	float distance = Length(closestPoint - sphere.center);

	// 距離が半径よりも小さければ衝突
	if (distance <= sphere.radius) {

		return true;
	}

	return false;
}

/// <summary>
/// AABBと線の当たり判定
/// </summary>
/// <param name="aabb"></param>
/// <param name="line"></param>
/// <returns></returns>
bool Collision::AABB2LineCheckCollision(const AABBInfo& aabb, const LineInfo& line) {

	float txMin = (aabb.min.x - line.origin.x) / (line.diff.x - line.origin.x);
	float txMax = (aabb.max.x - line.origin.x) / (line.diff.x - line.origin.x);
	if (txMin > txMax) std::swap(txMin, txMax);

	float tyMin = (aabb.min.y - line.origin.y) / (line.diff.y - line.origin.y);
	float tyMax = (aabb.max.y - line.origin.y) / (line.diff.y - line.origin.y);
	if (tyMin > tyMax) std::swap(tyMin, tyMax);

	float tzMin = (aabb.min.z - line.origin.z) / (line.diff.z - line.origin.z);
	float tzMax = (aabb.max.z - line.origin.z) / (line.diff.z - line.origin.z);
	if (tzMin > tzMax) std::swap(tzMin, tzMax);

	float tNear = std::max({ txMin, tyMin, tzMin });
	float tFar = std::min({ txMax, tyMax, tzMax });

	// 交差しているかを確認
	return (tNear <= tFar && tFar >= 0.0f && tNear <= 1.0f);
}

/// <summary>
/// OBBと球の当たり判定
/// </summary>
/// <param name="obb"></param>
/// <param name="sphere"></param>
/// <returns></returns>
bool Collision::OBB2SphereCheckCollision(const Vector3& rotate, const OBBInfo& obb, const SphereInfo& sphere) {

	// OBBの軸を回転させる
	Matrix4x4 rotateMatrix;
	Matrix4x4 rotateX = MakePitchMatrix(rotate.x);
	Matrix4x4 rotateY = MakeYawMatrix(rotate.y);
	Matrix4x4 rotateZ = MakeRollMatrix(rotate.z);
	rotateMatrix = Multiply(rotateX, Multiply(rotateY, rotateZ));

	Vector3 orientations[3];
	orientations[0] = Transform(obb.orientations[0], rotateMatrix);
	orientations[1] = Transform(obb.orientations[1], rotateMatrix);
	orientations[2] = Transform(obb.orientations[2], rotateMatrix);

	Vector3 localSphereCenter = sphere.center - obb.center;
	Vector3 closestPoint = obb.center;

	// 各軸ごとに処理を行う
	for (int i = 0; i < 3; ++i) {

		float distance = Dot(localSphereCenter, orientations[i]);
		float halfSize = (i == 0) ? obb.size.x : (i == 1) ? obb.size.y : obb.size.z;

		// クランプ処理
		if (distance > halfSize) {

			distance = halfSize;
		} else if (distance < -halfSize) {

			distance = -halfSize;
		}

		closestPoint += distance * orientations[i];
	}

	Vector3 diff = closestPoint - sphere.center;
	float distanceSquared = Dot(diff, diff);

	return distanceSquared <= (sphere.radius * sphere.radius);
}