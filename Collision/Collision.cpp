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
bool Collision::LinePlaneIntersection(Vector3& intersection, const Vector3& p0, const Vector3& p1, const Vector3& normal, const Vector3& pointOnPlane) {
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
/// 軸があるかどうか
/// </summary>
/// <param name="obb1"></param>
/// <param name="obb2"></param>
/// <returns></returns>
bool Collision::HasSeparatingAxis(const OBBInfo& obb1, const OBBInfo& obb2) {

	Vector3 axes[15];
	for (int i = 0; i < 3; ++i) {
		axes[i] = obb1.orientations[i];
		axes[i + 3] = obb2.orientations[i];
	}
	int index = 6;
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			axes[index++] = Cross(obb1.orientations[i], obb2.orientations[j]);
		}
	}

	for (int i = 0; i < 15; ++i) {
		if (IsSeparatingAxis(axes[i], obb1, obb2)) {
			return true;
		}
	}
	return false;
}
bool Collision::IsSeparatingAxis(const Vector3& axis, const OBBInfo& obb1, const OBBInfo& obb2) {

	float projection1 = GetProjectionRadius(axis, obb1);
	float projection2 = GetProjectionRadius(axis, obb2);
	float distance = std::fabs(Dot(axis, obb2.center - obb1.center));

	return distance > projection1 + projection2;
}
float Collision::GetProjectionRadius(const Vector3& axis, const OBBInfo& obb) {

	return obb.size.x * std::fabs(Dot(axis, obb.orientations[0])) +
		obb.size.y * std::fabs(Dot(axis, obb.orientations[1])) +
		obb.size.z * std::fabs(Dot(axis, obb.orientations[2]));
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
/// OBB同士の当たり判定
/// </summary>
/// <param name="obb1"></param>
/// <param name="obb2"></param>
/// <returns></returns>
bool Collision::OBB2OBBCheckCollision(const OBBInfo& obb1, const OBBInfo& obb2) {

	// SATによる分離軸の検出
	const float EPSILON = 1e-6f;

	// 15個の分離軸
	Vector3 axes[15];
	for (int i = 0; i < 3; ++i) {
		axes[i] = obb1.orientations[i];
		axes[i + 3] = obb2.orientations[i];
	}

	int axisIndex = 6;
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			axes[axisIndex] = Cross(obb1.orientations[i], obb2.orientations[j]);
			if (Length(axes[axisIndex]) > EPSILON) {
				axes[axisIndex] = Normalize(axes[axisIndex]);
			}
			++axisIndex;
		}
	}

	// 各軸に対する分離軸テスト
	for (int i = 0; i < 15; ++i) {
		Vector3 axis = axes[i];
		if (Length(axis) < EPSILON) {
			continue;
		}

		// OBB1を投影
		float obb1Min = INFINITY, obb1Max = -INFINITY;
		for (int j = 0; j < 8; ++j) {
			Vector3 vertex = obb1.center +
				obb1.orientations[0] * (j & 1 ? obb1.size.x : -obb1.size.x) +
				obb1.orientations[1] * (j & 2 ? obb1.size.y : -obb1.size.y) +
				obb1.orientations[2] * (j & 4 ? obb1.size.z : -obb1.size.z);
			float projection = Dot(vertex, axis);
			obb1Min = (std::min)(obb1Min, projection);
			obb1Max = (std::max)(obb1Max, projection);
		}

		// OBB2を投影
		float obb2Min = INFINITY, obb2Max = -INFINITY;
		for (int j = 0; j < 8; ++j) {
			Vector3 vertex = obb2.center +
				obb2.orientations[0] * (j & 1 ? obb2.size.x : -obb2.size.x) +
				obb2.orientations[1] * (j & 2 ? obb2.size.y : -obb2.size.y) +
				obb2.orientations[2] * (j & 4 ? obb2.size.z : -obb2.size.z);
			float projection = Dot(vertex, axis);
			obb2Min = (std::min)(obb2Min, projection);
			obb2Max = (std::max)(obb2Max, projection);
		}

		// 分離軸テスト
		if (obb1Max < obb2Min || obb2Max < obb1Min) {
			return false;
		}
	}

	// 全ての分離軸で交差しているので衝突している
	return true;
}

/// <summary>
/// AABBと球の当たり判定
/// </summary>
/// <param name="aabb1"></param>
/// <param name="aabb2"></param>
/// <returns></rleturns>
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

/// <summary>
/// OBBと線の当たり判定
/// </summary>
/// <param name="rotate"></param>
/// <param name="obb"></param>
/// <param name="line"></param>
/// <returns></returns>
bool Collision::OBB2LineCheckCollision(const Vector3& rotate, const OBBInfo& obb, const LineInfo& line) {

	// 回転行列の計算（オイラー角から回転行列を計算）
	Matrix4x4 rotateX = MakePitchMatrix(rotate.x);
	Matrix4x4 rotateY = MakeYawMatrix(rotate.y);
	Matrix4x4 rotateZ = MakeRollMatrix(rotate.z);
	Matrix4x4 rotateMatrix = Multiply(rotateX, Multiply(rotateY, rotateZ));

	// OBBの軸を回転させる
	Vector3 orientations[3];
	orientations[0] = Transform(obb.orientations[0], rotateMatrix);
	orientations[1] = Transform(obb.orientations[1], rotateMatrix);
	orientations[2] = Transform(obb.orientations[2], rotateMatrix);

	// 線分の始点と終点をOBBのローカル座標系に変換
	Vector3 lineStartLocal = line.origin - obb.center;
	Vector3 lineEndLocal = lineStartLocal + line.diff;

	// 各軸ごとにローカル座標に変換
	Vector3 lineStartTransformed = { Dot(lineStartLocal, orientations[0]), Dot(lineStartLocal, orientations[1]), Dot(lineStartLocal, orientations[2]) };
	Vector3 lineEndTransformed = { Dot(lineEndLocal, orientations[0]), Dot(lineEndLocal, orientations[1]), Dot(lineEndLocal, orientations[2]) };

	// 線分とAABBの当たり判定
	Vector3 boxMin = { -obb.size.x, -obb.size.y, -obb.size.z };
	Vector3 boxMax = { obb.size.x, obb.size.y, obb.size.z };

	// 線分とAABBの当たり判定を行う
	float tMin = 0.0f;
	float tMax = 1.0f;

	for (int i = 0; i < 3; ++i) {
		float start = (&lineStartTransformed.x)[i];
		float end = (&lineEndTransformed.x)[i];
		float min = (&boxMin.x)[i];
		float max = (&boxMax.x)[i];

		float t0 = (min - start) / (end - start);
		float t1 = (max - start) / (end - start);

		if (t0 > t1) std::swap(t0, t1);

		tMin = t0 > tMin ? t0 : tMin;
		tMax = t1 < tMax ? t1 : tMax;

		if (tMin > tMax) return false;
	}

	return true;
}