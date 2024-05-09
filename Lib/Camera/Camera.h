#pragma once
#include "MyMath.h"

/// <summary>
/// カメラクラス
/// </summary>
class Camera {
private:
	/// <summary>
	/// メンバ変数
	/// </summary>

	Matrix4x4 cameraMatrix_{};
	Matrix4x4 viewMatrix_{};
	Matrix4x4 orthoMatrix_{};
	Matrix4x4 projectionMatrix_{};
	Matrix4x4 viewportMatrix_{};

	Vector3 scale_{};
	Vector3 rotate_{};
	Vector3 translate_{};

	// 透視投影行列
	Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);
	// 正射影行列
	Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);
	// ビューポート変換行列
	Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);
public:
	/// <summary>
	/// メンバ関数
	/// </summary>

	// コンストラクタ
	Camera() {}
	// デストラクタ
	~Camera() {}

	void Init();
	void Update();

	/// <summary>
	/// ゲッター
	/// </summary>
	/// <returns></returns>
	Matrix4x4 GetViewMatrix() const { return viewMatrix_; }
	Matrix4x4 GetProjectionMatrix() const { return projectionMatrix_; }
	Matrix4x4 GetViewportMatrix() const { return viewportMatrix_; }
};