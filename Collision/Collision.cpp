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