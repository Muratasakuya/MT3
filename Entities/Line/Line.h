#pragma once
#include "MyMath.h"

/// <summary>
/// ラインクラス
/// </summary>
class Line {
private:

	struct Origin {

		Matrix4x4 worldMatrix;

		Vector3 ndcPos;
		Vector3 screenPos;
	};

	Origin origin_;

	struct Diff {

		Matrix4x4 worldMatrix;

		Vector3 ndcPos;
		Vector3 screenPos;
	};

	Diff diff_;

public:

	void DrawLine(const LineInfo& line, const Matrix4x4& viewMatrix, const Matrix4x4& projectionMatrix, const Matrix4x4& viewportMatrix);
};