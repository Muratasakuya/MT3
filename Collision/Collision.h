#pragma once
#include "MyMath.h"

/// <summary>
/// 衝突判定クラス
/// </summary>
class Collision{
private:
	/// <summary>
	/// メンバ変数
	/// </summary>

public:
	/// <summary>
	/// メンバ関数
	/// </summary>
	
	// 球と球の当たり判定
	bool Sphere2SphereCheckCollision(const SphereInfo& s1, const SphereInfo& s2);

	// 球と平面の当たり判定
	bool Sphere2PlaneCheckCollision(const SphereInfo& sphere, const PlaneInfo& plane);

	// 平面と線の当たり判定
	bool Plane2LineCheckCollision(const PlaneInfo& plane, const LineInfo& line);

	// 三角形と線の当たり判定
	bool Triangle2LineCheckCollision(const TriangleInfo& triangle, const LineInfo& line);

	// AABB同士の当たり判定
	bool AABB2AABBCheckCollision(const AABBInfo& aabb1, const AABBInfo& aabb2);

	// AABBと球の当たり判定
	bool AABB2SphereCheckCollision(const AABBInfo& aabb, const SphereInfo& sphere);

	// AABBと線の当たり判定
	bool AABB2LineCheckCollision(const AABBInfo& aabb, const LineInfo& line);
};