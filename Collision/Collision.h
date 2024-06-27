#pragma once
#include "MyMath.h"

/// <summary>
/// 衝突判定クラス
/// </summary>
class Collision {
private:
	/// <summary>
	/// メンバ変数
	/// </summary>

	// 交点計算
	static bool LinePlaneIntersection(Vector3& intersection, const Vector3& p0, const Vector3& p1, const Vector3& normal, const Vector3& pointOnPlane);

	// 軸があるか
	static bool HasSeparatingAxis(const OBBInfo& obb1, const OBBInfo& obb2);
	static bool IsSeparatingAxis(const Vector3& axis, const OBBInfo& obb1, const OBBInfo& obb2);
	static float GetProjectionRadius(const Vector3& axis, const OBBInfo& obb);

public:
	/// <summary>
	/// メンバ関数
	/// </summary>

	// 球と球の当たり判定
	static bool Sphere2SphereCheckCollision(const SphereInfo& s1, const SphereInfo& s2);

	// 球と平面の当たり判定
	static bool Sphere2PlaneCheckCollision(const SphereInfo& sphere, const PlaneInfo& plane);

	// 平面と線の当たり判定
	static bool Plane2LineCheckCollision(const PlaneInfo& plane, const LineInfo& line);

	// 三角形と線の当たり判定
	static bool Triangle2LineCheckCollision(const TriangleInfo& triangle, const LineInfo& line);

	// AABB同士の当たり判定
	static bool AABB2AABBCheckCollision(const AABBInfo& aabb1, const AABBInfo& aabb2);

	// OBB同士の当たり判定
	static bool OBB2OBBCheckCollision(const OBBInfo& obb1, const OBBInfo& obb2);

	// AABBと球の当たり判定
	static bool AABB2SphereCheckCollision(const AABBInfo& aabb, const SphereInfo& sphere);

	// AABBと線の当たり判定
	static bool AABB2LineCheckCollision(const AABBInfo& aabb, const LineInfo& line);

	// OBBと球の当たり判定
	static bool OBB2SphereCheckCollision(const Vector3& rotate,const OBBInfo& obb, const SphereInfo& sphere);

	// OBBと線の当たり判定
	static bool OBB2LineCheckCollision(const Vector3& rotate, const OBBInfo& obb, const LineInfo& line);
};