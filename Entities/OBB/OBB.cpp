#include "OBB.h"

#include "ImGuiManager.h"

/// <summary>
/// 初期化
/// </summary>
void OBB::Initialize() {

	rotate_ = { 0.0f,0.0f,0.0f };
}

/// <summary>
/// OBBの描画
/// </summary>
/// <param name="obb"></param>
/// <param name="viewProjectionMatrix"></param>
/// <param name="viewportMatrix"></param>
/// <param name="color"></param>
void OBB::DrawOBB(const OBBInfo& obb, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {

	ImGui::Begin("OBB");

	ImGui::DragFloat3("Rotation", &rotate_.x, 0.01f);

	ImGui::End();

	// 回転行列の計算（オイラー角から回転行列を計算）
	Matrix4x4 rotateX = MakePitchMatrix(rotate_.x);
	Matrix4x4 rotateY = MakeYawMatrix(rotate_.y);
	Matrix4x4 rotateZ = MakeRollMatrix(rotate_.z);
	rotateMatrix_ = Multiply(rotateX, Multiply(rotateY, rotateZ));

	Vector3 vertices[8];
	Vector3 halfSizeX = Transform(obb.orientations[0], rotateMatrix_) * obb.size.x;
	Vector3 halfSizeY = Transform(obb.orientations[1], rotateMatrix_) * obb.size.y;
	Vector3 halfSizeZ = Transform(obb.orientations[2], rotateMatrix_) * obb.size.z;

	// 頂点の相対位置を定義
	Vector3 offsets[8] = {

		{-1, -1, -1}, {-1,  1, -1}, {1, -1, -1}, {1,  1, -1},
		{-1, -1,  1}, {-1,  1,  1}, {1, -1,  1}, {1,  1,  1}
	};

	// 頂点を計算
	for (int i = 0; i < 8; ++i) {

		Vector3 localVertex = offsets[i].x * halfSizeX +
			offsets[i].y * halfSizeY +
			offsets[i].z * halfSizeZ;

		vertices[i] = obb.center + localVertex;
	}

	Vector3 points[8];
	for (int32_t index = 0; index < 8; ++index) {

		points[index] = Transform(Transform(vertices[index], viewProjectionMatrix), viewportMatrix);
	}

	// 頂点間の接続情報を定義
	int edges[12][2] = {

		{0, 1}, {1, 3}, {3, 2}, {2, 0}, // 前面
		{4, 5}, {5, 7}, {7, 6}, {6, 4}, // 背面
		{0, 4}, {1, 5}, {2, 6}, {3, 7}  // 側面
	};

	// ラインを描画
	for (int i = 0; i < 12; ++i) {

		int start = edges[i][0];
		int end = edges[i][1];

		Novice::DrawLine(
			static_cast<int>(points[start].x), static_cast<int>(points[start].y),
			static_cast<int>(points[end].x), static_cast<int>(points[end].y),
			color
		);
	}
}