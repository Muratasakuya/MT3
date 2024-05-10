#include "Line.h"

/// <summary>
/// ラインの描画
/// </summary>
/// <param name="line"></param>
/// <param name="viewMatrix"></param>
/// <param name="projectionMatrix"></param>
/// <param name="viewportMatrix"></param>
void Line::DrawLine(const LineInfo& line, const Matrix4x4& viewMatrix,
	const Matrix4x4& projectionMatrix, const Matrix4x4& viewportMatrix) {

	/****************************************************************************************************************************/
	// 行列変換

	origin_.worldMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f }, line.origin);
	diff_.worldMatrix=MakeAffineMatrix({ 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f }, line.diff);

	/****************************************************************************************************************************/
	// ndc変換

	origin_.ndcPos = Transform(line.origin, Multiply(viewMatrix, projectionMatrix));
	diff_.ndcPos = Transform(line.diff, Multiply(viewMatrix, projectionMatrix));

	/****************************************************************************************************************************/
	// screen変換

	origin_.screenPos = Transform(origin_.ndcPos, viewportMatrix);
	diff_.screenPos = Transform(diff_.ndcPos, viewportMatrix);

	// 描画
	Novice::DrawLine(
		static_cast<int>(origin_.screenPos.x), static_cast<int>(origin_.screenPos.y),
		static_cast<int>(diff_.screenPos.x), static_cast<int>(diff_.screenPos.y),
		line.color
	);
}