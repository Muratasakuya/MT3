#include "Camera.h"

/// <summary>
/// 透視投影行列
/// </summary>
/// <param name="fovY"></param>
/// <param name="aspectRatio"></param>
/// <param name="nearClip"></param>
/// <param name="farClip"></param>
/// <returns></returns>
Matrix4x4 Camera::MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip) {

	Matrix4x4 matrix = {};

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			matrix.m[i][j] = 0.0f;
		}
	}

	matrix.m[0][0] = 1.0f / (aspectRatio * std::tanf(fovY / 2.0f));
	matrix.m[1][1] = 1.0f / std::tanf(fovY / 2.0f);
	matrix.m[2][2] = farClip / (farClip - nearClip);
	matrix.m[2][3] = 1.0f;
	matrix.m[3][2] = (-farClip * nearClip) / (farClip - nearClip);

	return matrix;
}
/// <summary>
/// 正射影行列
/// </summary>
/// <param name="left"></param>
/// <param name="top"></param>
/// <param name="right"></param>
/// <param name="bottom"></param>
/// <param name="nearClip"></param>
/// <param name="farClip"></param>
/// <returns></returns>
Matrix4x4 Camera::MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip) {

	Matrix4x4 matrix = {};

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			matrix.m[i][j] = 0.0f;
		}
	}

	matrix.m[0][0] = 2.0f / (right - left);
	matrix.m[1][1] = 2.0f / (top - bottom);
	matrix.m[2][2] = 1.0f / (farClip - nearClip);
	matrix.m[3][0] = (left + right) / (left - right);
	matrix.m[3][1] = (top + bottom) / (bottom - top);
	matrix.m[3][2] = nearClip / (nearClip - farClip);
	matrix.m[3][3] = 1.0f;

	return matrix;
}
/// <summary>
/// ビューポート変換行列
/// </summary>
/// <param name="left"></param>
/// <param name="top"></param>
/// <param name="width"></param>
/// <param name="height"></param>
/// <param name="minDepth"></param>
/// <param name="maxSepth"></param>
/// <returns></returns>
Matrix4x4 Camera::MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth) {

	Matrix4x4 matrix = {};

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			matrix.m[i][j] = 0.0f;
		}
	}

	matrix.m[0][0] = width / 2.0f;
	matrix.m[1][1] = -height / 2.0f;
	matrix.m[2][2] = maxDepth - minDepth;
	matrix.m[3][0] = left + width / 2.0f;
	matrix.m[3][1] = top + height / 2.0f;
	matrix.m[3][2] = minDepth;
	matrix.m[3][3] = 1.0f;

	return matrix;
}

/// <summary>
/// 初期化
/// </summary>
void Camera::Init() {

	// アフィン
	scale_ = { 1.0f,1.0f,1.0f };
	rotate_ = { 0.26f,0.0f,0.0f };
	translate_ = { 0.0f,1.9f,-6.49f };

	cameraMatrix_ =
		MakeAffineMatrix(scale_, rotate_, translate_);
	viewMatrix_ = Inverse(cameraMatrix_);

	projectionMatrix_ =
		MakePerspectiveFovMatrix(0.45f, 1280.0f / 720.0f, 0.1f, 100.0f);
	viewportMatrix_ =
		MakeViewportMatrix(0.0f, 0.0f, 1280.0f, 720.0f, 0.0f, 1.0f);
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="keys"></param>
/// <param name="preKeys"></param>
void Camera::Update() {

	ImGui::Begin("Camera");

	ImGui::SliderFloat3("scale", &scale_.x, -1.0f, 1.0f);
	ImGui::SliderFloat3("rotate", &rotate_.x, -1.0f, 1.0f);
	ImGui::SliderFloat3("translate", &translate_.x, -10.0f, 10.0f);

	ImGui::End();

	cameraMatrix_ =
		MakeAffineMatrix(scale_, rotate_, translate_);
	viewMatrix_ = Inverse(cameraMatrix_);
}