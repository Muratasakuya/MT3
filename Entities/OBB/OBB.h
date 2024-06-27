#pragma once
#include <Novice.h>
#include "MyMath.h"

/// <summary>
/// OBBクラス
/// Oriented Bounding Box
/// </summary>
class OBB {
private:
	/// <summary>
	/// メンバ変数
	/// </summary>
	
	Matrix4x4 rotateMatrix_{};
	Matrix4x4 worldMatrix_{};
	Matrix4x4 wvpMatrix_{};

	Vector3 rotate_{};
	Vector3 translate_{};

public:
	/// <summary>
	/// メンバ関数
	/// </summary>

	// OBBの描画
	void DrawOBB(const OBBInfo& obb, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);

	// getter

	Matrix4x4 GetWorldMatrix() const { return worldMatrix_; }
	Vector3 GetRotate() const { return rotate_; }

	// setter

	void SetTranslate(Vector3 translate) { translate_ = translate; }
	void SetRotate(Vector3 rotate) { rotate_ = rotate; }
};