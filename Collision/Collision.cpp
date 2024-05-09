#include "Collision.h"

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
	}
	else {

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
	}
	else {

		return false;
	}
}